#ifndef AT_Header_User_Defined_H_INCLUDED
#define AT_Header_User_Defined_H_INCLUDED

//----------------------------------------Definitions - User Defined---------------------
//--------------------Date
struct simpledate{
    unsigned int year, month, day;
};

//--------------------Historical Prices
struct historicalprices {
    struct simpledate date;
    float open, high, low, close, adjclose;
    unsigned long volume;
};

//--------------------Price Entry
struct price_entry{
    long secpass;
    float price;
};

//--------------------Positions
struct positions{
    string ticker;
    int num;
};

//--------------------Accout infomation
struct account_info{
    vector <positions> stocks;
    double cash;
    int num_pos;

    account_info();
    ~account_info(){;};
    double stock_value();
    double total_value();
    bool trade(string ticker, float price, int num);
    void display_account_info();
    int num_stocks(string ticker);
};

//--------------------Current Data Info
class current_date_info{
    private:
        unsigned int year, month, wday, mday, yday;

    public:
        current_date_info();
        ~current_date_info(){;};
        unsigned int getyear(){return year;};
        unsigned int getmonth(){return month;};
        unsigned int getwday(){return wday;};
        unsigned int getmday(){return mday;};
        unsigned int getyday(){return yday;};
};

//--------------------Message Board
class message_board{
    public:
        string message;
        bool lock;

        message_board(){message = ""; lock =false;};
        ~message_board(){;};
        string return_message(){return message;};
        bool add_message(string newmessage){
            if(lock ==false){
                lock ==true;
                message += newmessage;
                lock = false;
                return true;
            }
            else
                return false;
        };
        void operator <<(char* newmessage){
                message += newmessage;
        };
};
//---------------------------------------------------------------------------------------

#endif // AT_Header_User_Defined_H_INCLUDED
