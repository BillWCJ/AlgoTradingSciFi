//----------------------------------------Prototypes-------------------------------------
void display_tickers(vector <string> & tickers, unsigned int ticker_count);
void display_time();
void display_historical_price(struct historicalprices price);
//---------------------------------------------------------------------------------------

//----------------------------------------Display Functions------------------------------
//--------------------Display Tickers
void display_tickers(vector <string> & tickers, unsigned int ticker_count){
    cout << "Ticker count is:" << ticker_count << endl;
    for(int i =0; i < tickers.size(); i++)
        cout << "Ticker " <<i+1 << ":" << tickers[i] << endl;
    cin.get();
}

//--------------------Display Time
void display_time(){
    time_t rawtime;
    struct tm timeinfo;
    char time_buffer[80];
    time(&rawtime);
    timeinfo = *localtime(&rawtime);
    strftime (time_buffer, 80, "%X | %x | %w",&timeinfo);
    cout << time_buffer<< endl;
}

//--------------------Display Historical Price
void display_historical_price(struct historicalprices price){
    cout << price.date.day << "-";
    cout << price.date.month << "-";
    cout << price.date.year << ",";
    cout << price.open << "," << price.high << "," << price.low <<",";
    cout << price.close <<"," << price.volume << "," << price.adjclose << endl;
}
//---------------------------------------------------------------------------------------
