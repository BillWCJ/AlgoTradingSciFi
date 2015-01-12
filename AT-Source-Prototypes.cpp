//----------------------------------------Prototypes-------------------------------------
int alias_main();

void error (CURLcode res);
static void dump (const char *text, FILE *stream, unsigned char *ptr, size_t size, char nohex);
static int my_trace (CURL *handle, curl_infotype type, char *data, size_t size, void *userp);
static size_t WriteMemoryCallback (void *contents, size_t size, size_t nmemb, void *userp);

unsigned int read_in_tickers(vector <string> &tickers);
bool repeated_ticker(vector <string> &tickers, string temp);
bool read_in_all_historical_prices(unsigned int num_read,vector <string> tickers,vector <vector <struct historicalprices> > &previous_price);
void display_historical_price(struct historicalprices price);

void display_tickers(vector <string> & tickers, unsigned int ticker_count);
void display_time();
void display_historical_price(struct historicalprices price);

bool read_command(string &command, bool &termination);
string process_info(char * memory, string ticker);
struct price_entry retrieve_price(string ticker, struct price_entry &current);

unsigned int yday(simpledate request_date);
void set_to_zero(float floatarray[]);
void set_to_one(float floatarray[]);
double SMA(int num, float prices[]);
unsigned int period_consider(int num, float signif);
void output_recent(string ticker, float min_prices[]);
