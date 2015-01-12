//----------------------------------------Prototypes-------------------------------------
bool read_command(string &command, bool &termination);
string process_info(char * memory, string ticker);
struct price_entry retrieve_price(string ticker, struct price_entry &current);
//---------------------------------------------------------------------------------------

//----------------------------------------Independent Functions--------------------------
//--------------------Read Command
bool read_command(string &command, bool &termination){
        cout <<"\r"<<emptyl;
        cout <<"\r";
        cout << "Enter Command:";
        cin >> command;

		if(command =="quit"){
            cout <<endl<< "Quiting!!!"<<endl;
            termination =true;
		}
        fflush(stdin);
}

//--------------------Process Info
string process_info(char * memory, string ticker){
    //----------Process Stock Price
    string source = memory;//cope memory into source
    if(source.size() >0){
        //specify the string which need to find the price and find price
        string search = "id=\"yfs_l84_" + ticker + "\">";
        long pos = source.find(search,5000)+search.size();
        long len = source.find("<", pos) - pos;
        search = source.substr(pos,len);

        return search;

        //find market time
        search = "market_time\">";
        pos = source.find(search,5000)+search.size();
        len = source.find("<", pos) - pos;
        search = source.substr(pos,len);

        cout << search << endl;
    }
    else
        return "0";
}

//--------------------Retrieve Price
struct price_entry retrieve_price(string ticker, struct price_entry &current){
    CURL *curl_handle = curl_easy_init(); //init the curl session
    if(curl_handle){
        CURLcode result;
        string address = "http://download.finance.yahoo.com/d/quotes.csv?s=" + ticker + "&f=p";
        time_t rawtime;
        struct tm retrieve_time;
        struct MemoryStruct chunk;
        chunk.memory = (char*)malloc(1); //will be grown as needed by the realloc above
        chunk.size = 0; //no data at this point

        //----------Retrieve Stock Price
        curl_easy_setopt(curl_handle, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, '5');
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

        time(&rawtime);
        retrieve_time = *localtime(&rawtime);

        result = curl_easy_perform(curl_handle);
        if(result != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s",curl_easy_strerror(result));
            current.price =0;
            current.secpass =-1;
            display_time();
            if(chunk.memory)
                free(chunk.memory);
            curl_easy_cleanup(curl_handle);
            return current;
        }
        else{
            stringstream(chunk.memory)>>current.price;
            current.secpass = (retrieve_time.tm_hour-9)*3600 + (retrieve_time.tm_min-30)*60 + retrieve_time.tm_sec;
            if(chunk.memory)
                free(chunk.memory);
            curl_easy_cleanup(curl_handle);

            //cout << "bytes retrieved: " << (long)chunk.size << endl;
            //cout << "Price: " << current.price << endl;
            //cout << "Time: " << retrieve_time.tm_hour << ":" << retrieve_time.tm_min << ":" <<retrieve_time.tm_sec <<endl;
            //cout << "Seconds Passed Since Market Open: " << current.secpass << endl;
            numtrans++;
            return current;
        }
    }
    else {
        current.price =0;
        current.secpass =-1;
        cout << "Curl Handler Error, No Data Retrieved: "<<ticker <<" ";
        display_time();
        return current;
    }
}
//---------------------------------------------------------------------------------------
