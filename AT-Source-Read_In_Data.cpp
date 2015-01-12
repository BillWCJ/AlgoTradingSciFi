//----------------------------------------Prototypes-------------------------------------
unsigned int read_in_tickers(vector <string> &tickers);
bool repeated_ticker(vector <string> &tickers, string temp);
bool read_in_all_historical_prices(unsigned int num_read,vector <string> tickers,vector <vector <struct historicalprices> > &previous_price);
bool read_in_historical_prices(unsigned int days, string ticker, vector <struct historicalprices> & prev_price);
//---------------------------------------------------------------------------------------

//----------------------------------------Read In Data Functions-------------------------
//--------------------Read in Ticker
unsigned int read_in_tickers(vector <string> &tickers){
    string temp;
    string ticker_file = "AT-Data/ticker.txt";
    unsigned ticker_count =0;

    //----------Read in Tickers
    ifstream in_ticker(ticker_file.c_str());
    if (in_ticker){
        while (!in_ticker.eof()){
            in_ticker >> temp;
            if (temp !="" && temp != "/n" && temp !=" " && !repeated_ticker(tickers, temp)) {
                tickers.resize(++ticker_count);
                tickers[ticker_count-1] = temp;
            }
        }
        in_ticker.close();
    }
    return ticker_count;
}

//--------------------Repeated Ticker
bool repeated_ticker(vector <string> &tickers, string temp){
    unsigned int n = tickers.size();
    for (int i=0; i<n ; i++)
        if (tickers[i] == temp)
            return true;
    return false;
}

//--------------------Read in All Historical Prices
bool read_in_all_historical_prices(unsigned int num_read,vector <string> tickers,vector <vector <struct historicalprices> > &previous_price){
    for(int i =0; i < ticker_count; i++){
        previous_price.resize(i+1);
        read_in_historical_prices(1000,tickers[i],previous_price[i]);
    }
}

//--------------------Read in Historical Price
bool read_in_historical_prices(unsigned int days, string ticker, vector <struct historicalprices> & prev_price){
    string filename = "AT-Data/Stock Info/" +ticker +".csv";
    ifstream in(filename.c_str());
    if(in){
        cout << "Sucessfully Opened " << ticker << endl;
        string temp;
        int pos1=0, pos2=0;
        getline(in,temp);

        for(unsigned int i =0; i <days&& !in.eof(); i++){
            getline(in, temp);
            if(temp != " " && temp != "\n" && temp !=""){
                prev_price.resize(i+1);
                pos1=0;
                pos2=temp.find("-");
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.day;
                pos1 = pos2+1;
                pos2 = temp.find("-",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.month;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.year;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].open;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].high;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].low;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].close;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].volume;
                pos1 = pos2+1;
                pos2 = temp.find(",",pos1);
                stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].adjclose;
            }
        }
        in.close();
        return true;
    }
    else {
        cout << "Cannot Open "<<ticker<< endl;
        cout << "Program Will Attempt To Download Historical Prices" << endl;

        CURL *curl_handle = curl_easy_init(); //init the curl session
        FILE*file = fopen(filename.c_str(), "w");
        if(curl_handle && file){
            CURLcode result;
            string address = "http://ichart.yahoo.com/table.csv?s=" + ticker + "&a=0&b=1&c=1990&g=d&ignore=.csv";

            //----------Retrieve CSV
            curl_easy_setopt(curl_handle, CURLOPT_URL, address.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 50000);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, file);
            result = curl_easy_perform(curl_handle);
            if(result != CURLE_OK){
                fprintf(stderr, "curl_easy_perform() failed: %s",curl_easy_strerror(result));
                display_time();
                curl_easy_cleanup(curl_handle);
                return false;
            }
            else{
                curl_easy_cleanup(curl_handle);
                fclose(file);
                cout <<"Sucessfully Downloaded " <<ticker <<".csv" << endl;
                ifstream in(filename.c_str());
                if(in){
                    cout << "Sucessfully Opened " << ticker << endl;
                    string temp;
                    int pos1=0, pos2=0;
                    getline(in,temp);

                    for(unsigned int i =0; i <days&& !in.eof(); i++){
                        getline(in, temp);
                        if(temp != " " && temp != "\n" && temp !=""){
                            prev_price.resize(i+1);
                            pos1=0;
                            pos2=temp.find("-");
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.day;
                            pos1 = pos2+1;
                            pos2 = temp.find("-",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.month;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].date.year;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].open;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].high;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].low;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].close;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].volume;
                            pos1 = pos2+1;
                            pos2 = temp.find(",",pos1);
                            stringstream(temp.substr(pos1,pos2-pos1))>>prev_price[i].adjclose;
                        }
                    }
                    in.close();
                    return true;
                }
            }
        }
        else {
            cout << "Curl Handler Error, No Data Retrieved: "<<ticker <<" ";
            display_time();
            return false;
        }
        return false;
    }
}
//---------------------------------------------------------------------------------------
