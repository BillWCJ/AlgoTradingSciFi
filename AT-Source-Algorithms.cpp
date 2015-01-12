//----------------------------------------Prototypes-------------------------------------
unsigned int yday(simpledate request_date);
void set_to_zero(float floatarray[]);
void set_to_one(float floatarray[]);
double SMA(int num, float prices[]);
unsigned int period_consider(int num, float signif);
void output_recent(string ticker, float min_prices[]);
//---------------------------------------------------------------------------------------

//----------------------------------------Independent Functions--------------------------
unsigned int yday(simpledate request_date){
    struct tm request ;
    request.tm_mday = request_date.day;
    request.tm_mon= request_date.month-1;
    request.tm_year = request_date.year - 1900;
    request.tm_sec = 0;
    request.tm_min =0;
    request.tm_hour =0;
    if (mktime(&request) == -1)
        cout << "Error in yday" << endl;
    return request.tm_yday;
}

void set_to_zero(float floatarray[]){
    unsigned int size = sizeof(floatarray);
    for(unsigned int i; i < size; i++)
        floatarray[i] = 0;

}

void set_to_one(float floatarray[]){
    unsigned int size = sizeof(floatarray);
    for(unsigned int i; i < size; i++)
        floatarray[i] = 1;

}

double SMA(int num, float prices[]){
    double sma=0;
    for (int i; i < num; i++)
        sma += prices[i];
    return sma /= num;
}

unsigned int period_consider(int num, float signif){
    const double k = 2/(num+1);
    unsigned int num_consid = 0;
    double signif_now = 1;
    if(signif < 0.05)
        signif = 0.05;

    while(signif_now > signif){
        signif_now = 1 - signif_now*k;
        num_consid++;
    }
    return num_consid;
}

void output_recent(string ticker, float min_prices[]){
    string todaystring = static_cast<ostringstream*>( &(ostringstream() << today.getyear()) )->str()+"-";
    if (today.getmonth() <10)
        todaystring +="0";
    todaystring += static_cast<ostringstream*>( &(ostringstream() << today.getmonth()) )->str()+"-";
    if (today.getmday() <10)
        todaystring +="0";
    todaystring += static_cast<ostringstream*>( &(ostringstream() << today.getmday()) )->str();

    ticker+="_recent.csv";
    double helper=0;
    ofstream out(ticker.c_str());

    for(int i =0; i< 390; i++){
        helper = i;
        out <<todaystring << ",";
        if (floor(helper /60 + 9.5) <10)
            out << 0;
        out << floor(helper /60 + 9.5) <<":";
        if ((int)floor((helper/60 + 9.5)*60) %60 < 10)
            out << 0;
        out << (int)floor((helper/60 + 9.5)*60)%60<<":";
        out << "00," << min_prices[i] << endl;
    }
    out.close();
}
