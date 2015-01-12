//----------------------------------------Global Variables-------------------------------
vector <string> tickers;//access by both main and autopilot_trading!!!
vector <vector <struct historicalprices> > previous_price;//access by both main and autopilot_trading!!!
unsigned int ticker_count = 0;
clock_t clocktime;
bool termination = false;
struct account_info account;//access by both main and autopilot_trading!!!
class current_date_info today;
long numtrans=0;
int testshift = 0;
const char linel[41] ="----------------------------------------";
const char emptyl[41]="                                        ";
//---------------------------------------------------------------------------------------
