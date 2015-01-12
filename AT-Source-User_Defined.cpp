//----------------------------------------Class & Structure Functions--------------------
//--------------------Account Info
    account_info :: account_info(){
        ifstream in("AT-Data/account.txt");
        if (in){
            in >> cash;
            string temp;
            int counter=0;
            while (!in.eof()){
                in >> temp;
                if(temp != ""&& temp!= " " && temp != " \n"){
                    stocks.resize(++counter);
                    stocks[counter-1].ticker = temp;
                    in >> stocks[counter-1].num;
                }
            }
            in.close();
        }
        num_pos=stocks.size();
    }

    double account_info :: stock_value(){
        double value =0;
        struct price_entry current;
        for(int i = 0; i < num_pos; i++){
            retrieve_price(stocks[i].ticker, current);
            value += stocks[i].num * current.price ;
        }
        return value;
    };

    double account_info :: total_value(){
        return cash + stock_value();
    };

    bool account_info :: trade(string ticker, float price, int num){
        if(cash - price*num >0){
            for(int i = 0; i < num_pos; i++){
                if(stocks[i].ticker == ticker){
                    stocks[i].num += num;
                    cash -= price*num;
                    return true;
                }
            }
            stocks.resize(++num_pos);
            stocks[num_pos-1].ticker = ticker;
            stocks[num_pos-1].num = num;
            return true;
        }
        else return false;
    };

    void account_info :: display_account_info(){
        cout << linel << endl;
        cout << "Stock Value: $" << fixed <<setprecision(2) << left<< stock_value() << endl;
        cout << "Cash Value: $" << fixed<< setprecision(2)  << left<< cash << endl;
        cout << "Total Value: $" << fixed<< setprecision(2)  << left<< total_value() << endl;
        cout << linel<<endl;
    }

    int account_info :: num_stocks(string ticker){
        for(int i = 0; i < num_pos; i++)
            if(stocks[i].ticker == ticker)
                return stocks[i].num;
    };

//--------------------Current Data Info
    current_date_info :: current_date_info(){
            time_t rawtime;
            struct tm timeinfo;
            time(&rawtime);
            timeinfo = *localtime(&rawtime);
            wday = timeinfo.tm_wday;
            mday = timeinfo.tm_mday;
            yday = timeinfo.tm_yday;
            month = timeinfo.tm_mon+1;
            year = timeinfo.tm_year +1900;
    };
//---------------------------------------------------------------------------------------
