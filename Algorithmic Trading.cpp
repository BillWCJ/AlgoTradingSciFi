//----------------------------------------Inlcude Header & Source Files------------------
#include "AT-Header-Includes.h"
#include "AT-Header-Curl.h"
#include "AT-Header-User_Defined.h"
#include "AT-Source-Prototypes.cpp"
#include "AT-Source-Global_Variables.cpp"
#include "AT-Source-Curl.cpp"
#include "AT-Source-User_Defined.cpp"
#include "AT-Source-Yahoo_API.cpp"
#include "AT-Source-Read_In_Data.cpp"
#include "AT-Source-Display.cpp"
#include "AT-Source-Functions.cpp"
#include "AT-Source-Algorithms.cpp"

//----------------------------------------Prototypes-------------------------------------
int alias_main();

//----------------------------------------Functions In Development-----------------------
bool format_price_days(vector <struct historicalprices> prev_price, float formated_price[]){
    unsigned int lastcounter= 0, num_prev = prev_price.size(), num_form = sizeof(formated_price);
    for(unsigned int i = 0; i < num_form; i++){
        for(unsigned int j=0; j < num_prev; j++){
            if (yday(prev_price[j].date) == today.getyday()-lastcounter){
                formated_price[i] = prev_price[j].close;
            }
            lastcounter ++;
        }
    }
}

double EMA(int num, float signif, float cu_price, float formated_price[]){
    const double k = 2/(num+1);
    unsigned int num_consid = period_consider(num,signif);
    float factor [num_consid];
    set_to_one(factor);
    unsigned int counter =0;
    float factor_left = 1;
    factor[0] = k;

    for(int i =1; i < num_consid; i++){
        factor_left -= factor[i-1];
        factor[i] = factor_left*k;
    }
    factor_left =0;
    for(int i =0; i < num_consid; i++){
        factor_left += factor[0];
    }
    factor_left = 1 / factor_left;
    for(int i =0; i < num_consid; i++){
        factor[i] *= factor_left;
    }
    double ema= cu_price * k;
    for(int i =1; i < num_consid-1; i++){
        ema+=formated_price[i] * factor[i];
    }
    return ema;
}

float MACD(float signif, int factor, int sp, int lp, int empp, float cu_price, vector <struct historicalprices> prev_price, vector <struct price_entry> prices){
    double dlp, dsp, ds;

    if (factor ==1){
        unsigned int num_consid = period_consider(lp,signif);
        float formatted_price[num_consid-1];
        set_to_zero(formatted_price);
        format_price_days(prev_price, formatted_price);
        dlp = EMA(lp, signif, cu_price, formatted_price);
        dsp = EMA(sp, signif, cu_price,formatted_price);
        dsp -=dlp;
    }
}

void *autopilot_trading(void *arg){
    int  threadnum = *(int*)arg;
    cout << "Thread " << threadnum << " trading " << tickers[threadnum] << " Starts"<< endl;
    //----------Double Up Data
    string ticker = tickers[threadnum];
    vector <struct historicalprices> prev_price;
    prev_price = previous_price[threadnum];
    vector <struct price_entry> sec_price(30);
    struct price_entry tempentry;
    time_t rawtime;
    struct tm timer;

    time(&rawtime);
    timer = *localtime(&rawtime);
    //ostringstream out,out2;

    int counter =0, last = -1;
    int num_record =0;
    double total=0;

    float min_prices [390] = {0};
    int sec_counter =0;
    int discot =0;
    int forcount =0;
    //out2 <<setw(5) <<left << ticker <<": ";

    do{
        if(termination)
            break;
        discot ++;
        if(discot >100){
            //out <<"Thread "<<ticker <<" Still Working" <<endl;
            discot -=100;
        }

        time(&rawtime);
        timer = *localtime(&rawtime);

        counter = (timer.tm_hour-9)*3600 + (timer.tm_min-30)*60 + timer.tm_sec;
        counter += testshift;




        if( counter <0 ){
            //out << "Thread " <<ticker << " Pausing" <<endl;
            Sleep(0-counter);
        }
        else if (counter > last+60 && sec_counter >0){
            num_record =0;
            total =0;
            for (int i = 0; i < 30; i ++){
                if (sec_price[i].secpass +testshift> counter-60&& sec_price[i].price>0){
                    total += sec_price[i].price;
                    num_record++;
                }
            }
            last = counter;
            total /= num_record;
            min_prices[(int)floor(counter /60)] = total;
            //out<< ticker <<" Price Stored: $" <<total << endl;
            sec_counter =0;
        }
        else if(sec_counter >0 && counter >= sec_price[sec_counter -1].secpass+2){
            retrieve_price(ticker,sec_price[sec_counter]);
            //out2 << setw(5) <<right << sec_price[sec_counter].price <<"(" << setw(6)<<sec_price[sec_counter].secpass<<"),";
            sec_counter++;
            Sleep(5000);
        }
        else if(sec_counter ==0){
            retrieve_price(ticker,sec_price[sec_counter]);
            //out2 << setw(5) <<right << sec_price[sec_counter].price <<"(" << setw(6)<<sec_price[sec_counter].secpass<<"),";
            sec_counter++;
            Sleep(5000);
        }
        else
            Sleep(5000);


        //if(++forcount >3){
            //forcount =0;
            //out2<<endl;
            //notif+=out2.str();
            //out2.flush();
            //out2 <<setw(5) <<left << ticker <<": ";
        //}

        //notif += out.str();
        //out.flush();
        if(sec_counter >29)
            sec_counter = 0;


    }while (counter/60 -0.5 < 390);


    output_recent(ticker, min_prices);

    cout<< ticker<< " Thread Done Saving Data\n";
    //notif += out.str();
}

//----------------------------------------Independent Functions--------------------------
//--------------------Main
int main (int argc, char *argv[]) {
    //Test Shift
    cout <<"WHAT IS THE TEST SHIFT VALUE" <<endl;
    cin >>testshift;
    system("cls");
    alias_main();
}

//--------------------Alias Main
int alias_main(){
    //----------Set up & Initialization
    long tra=0;
    time_t rawtime;
    struct tm timer;
    string command;
    unsigned const int year = today.getyear(), month = today.getmonth(), day = today.getmday();
    unsigned int hour=0, minute=0, second=0;
    unsigned int lasthour=100, lastminute=100, lastsecond=100;
    curl_global_init(CURL_GLOBAL_ALL);
    ticker_count = read_in_tickers(tickers);
    read_in_all_historical_prices(1000,tickers,previous_price);
    ostringstream buffer;
    account.display_account_info();

    //----------Create Threads For Each Stock
    int trackers[ticker_count];
    int *tracker_pointer[ticker_count];
    //pthread_t stock_thread[ticker_count];

    //----------Start Each Thread
    for (int i =0; i < ticker_count; i++){
        tracker_pointer[i] = NULL;
        if(previous_price[i].size() > 0 && 1==1/*some other check*/){
            trackers[i] = i;
            tracker_pointer[i] = &trackers[i];
            buffer <<"Start Thread :" <<i <<" : " << tickers[i] << endl;
            //pthread_create(&stock_thread[i],NULL,autopilot_trading,tracker_pointer[i]);
        }
    }

    //----------Stand By Mode
    cout << linel <<endl;
    cout << "(All Thread Should Have Started. Main ON StandBy)" << endl;
    cout <<"Main On Standy By: "<<year <<"/"<<setw(2)<<setfill('0')<<month<<"/"<<setw(2)<<setfill('0')<<day<<" - ";

    do{
        time(&rawtime);
        timer = *localtime(&rawtime);
        hour = timer.tm_hour;
        minute = timer.tm_min;
        second = timer.tm_sec;

        if(hour != lasthour || minute != lastminute || second != lastsecond){
            if(hour != lasthour || minute != lastminute){
                for (int i=0; i<8; i++)
                    cout<< "\b";
                cout<<setw(2)<<setfill('0')<<left<<hour<<":"<<setw(2)<<setfill('0')<<left<<minute<<":"<<setw(2)<<setfill('0')<<left<<second;
                lastminute = minute;
                lasthour = hour;
            }
            if(second != lastsecond){
                for (int i=0; i<2; i++)
                    cout<< "\b";
                cout<<setw(2)<<setfill('0')<<left<<second;
                lastsecond = second;
            }
        }
        else
            Sleep(250+tra);

//        if (++tra >10){
//            tra =0;
//            cout <<"\r"<<emptyl;
//            cout <<"\r";
//        }

        if(kbhit())
            read_command(command,termination);
		if(termination)
            break;
    }while(termination ==false);


    //----------Terminate Thread
    cout << linel<<endl;
	cout <<"Program Termination. Begin Thread Termination" << endl;
    cout <<"Wait For all Thread to Terminate" << endl;

    for (int i =0; i < ticker_count; i++){
        if(tracker_pointer[i]){
            //pthread_join(stock_thread[i],NULL);
            cout << "Thread: " << i <<" : "<<setw(8) << setfill(' ') << tickers[i] <<" Terminated" << endl;
        }
        else
            cout << "Thread: " << i <<" : "<< setw(8) <<setfill(' ')<< tickers[i] <<" Already Terminated/Never Started" << endl;
    }

    //----------Clean Up
    curl_global_cleanup();

    return 0;
}

