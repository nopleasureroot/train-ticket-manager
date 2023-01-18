#include <iostream>
#include <fstream>
#include "vector"

using namespace std;

/* char* fio
 * char* point_from
 * char* point_to
 * double price
 *
 * functions:
 * 1) find ticket by fio +
 * 2) Filter by price (ascending/descending) +
 * 3) Sort by char's
 * 4) static variable for count entity +
 * 5) Data placed is dynamic memory +
 * 6) add new element +
 * 7) print data +
 * 8) exit from program +
**/

static int entity_counter = 0;

enum sort_type {
	asc_price,
	desc_price,
	asc_fio,
	desc_fio
};

struct train_ticket {
	char fio[40];
	char point_from[40];
	char point_to[40];
	double price;

	bool operator > (const train_ticket& str) const
	{
		return (price > str.price);
	}

	bool operator < (const train_ticket& str) const
	{
		return (price < str.price);
	}
};

struct fio_comparator_asc {
	bool operator() (const train_ticket& lStr, const train_ticket& rStr) const
	{
		return strcmp(lStr.fio, rStr.fio) > 0;
	}
};

struct fio_comparator_desc {
	bool operator() (const train_ticket& lStr, const train_ticket& rStr) const
	{
		return strcmp(lStr.fio, rStr.fio) < 0;
	}
};

vector<train_ticket> train_tickets;

static string FILE_PATH = "data.txt";

namespace common_util {
	static vector<string> split(const string& str, const string& delim)
	{
		vector<string> tokens;
		size_t prev = 0, pos;
		do
		{
			pos = str.find(delim, prev);
			if (pos == string::npos) pos = str.length();
			string token = str.substr(prev, pos-prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		}
		while (pos < str.length() && prev < str.length());
		return tokens;
	}
}

namespace input_util {
	static string LINE = "\n------------------------------------------------------------------------------------------";
	static string FIO = "\n\t\t\tPlease enter your name, middle-name, surname using space: ";
	static string POINT_FROM = "\t\t\tPlease enter point from place: ";
	static string POINT_TO = "\t\t\tPlease enter point to place: ";
	static string PRICE = "\t\t\tPlease enter price: ";
	static string FIND_TICKET = "\t\t\tPlease enter fio from ticket: ";

	static void print_fio() {
		cout << LINE;
		cout << FIO;
	}

	static void print_point_from() {
		cout << POINT_FROM;
	}

	static void print_point_to() {
		cout << POINT_TO;
	}

	static void print_price() {
		cout << PRICE;
	}
}

namespace output_util {
	static string LINE = "\n\t\t\t----------------------------------------------------";
	static string NEW_LINE = "\n";

	static string FIO_COLUMN_NAME = "\t\t\tFIO: ";
	static string TOWN_FROM_COLUMN_NAME = "\t\t\tTown from: ";
	static string TOWN_TO_COLUMN_NAME = "\t\t\tTown to: ";
	static string PRICE_COLUMN_NAME = "\t\t\tPrice: ";

	static string PRINT_TICKETS = "\n\t\t\tAll tickets has been printed";
	static string COUNT_ENTITIES = "\t\t\tCount entities: ";

	static string COMMAND_NOT_FOUND = "\nPlease type other commands which present at menu! \n";
	static string ENTER_FOR_CONTINUE = "\n\n\n\t\t\tPress enter to go back \n\n";
	static string TICKET_NOT_FOUND = "\t\t\tTicket not found. Please enter existing passenger";

	static string MENU_ONE = "\n\t\t\t>>>>>>>>>  TRAIN TICKET MANAGEMENT SYSTEM  <<<<<<<<<";
	static string MENU_TWO = "\n\t\t\tENTER   1:   To View List of Tickets";
	static string MENU_THREE = "\n\t\t\tENTER   2:   To View Ticket by fio";
	static string MENU_FOUR = "\n\t\t\tENTER   3:   To Add New Ticket";
	static string MENU_FIVE = "\n\t\t\tENTER   4:   To asc sort tickets by price";
	static string MENU_SIX = "\n\t\t\tENTER   5:   To desc sort tickets by price";
	static string MENU_SEVEN = "\n\t\t\tENTER   6:   To asc sort tickets by fio";
	static string MENU_EIGHT = "\n\t\t\tENTER   7:   To desc sort tickets by fio";

	static string MENU_NINE = "\n\t\t\tENTER   8:   To Exit     ";
	static string MENU_TEN = "\n\n\t\t\tPlease Enter Your Choice: ";

	static void print_ticket(struct train_ticket ticket) {
		cout << LINE << NEW_LINE;
		cout << FIO_COLUMN_NAME << ticket.fio << NEW_LINE;
		cout << TOWN_FROM_COLUMN_NAME << ticket.point_from << NEW_LINE;
		cout << TOWN_TO_COLUMN_NAME << ticket.point_to << NEW_LINE;
		cout << PRICE_COLUMN_NAME << ticket.price << NEW_LINE;
	}

	static void print_empty() {
		cout << LINE << NEW_LINE;
		cout << TICKET_NOT_FOUND << NEW_LINE;
	}

	static void print_menu() {
		cout << MENU_ONE;
		cout << NEW_LINE;
		cout << LINE;
		cout << MENU_TWO;
		cout << LINE;
		cout << MENU_THREE;
		cout << LINE;
		cout << MENU_FOUR;
		cout << LINE;
		cout << MENU_FIVE;
		cout << LINE;
		cout << MENU_SIX;
		cout << LINE;
		cout << MENU_SEVEN;
		cout << LINE;
		cout << MENU_EIGHT;
		cout << LINE;
		cout << MENU_NINE;
		cout << LINE;
		cout << MENU_TEN;
	}
}

/**
 * input block method
 */
void wait_for_enter(){
	cout << output_util::ENTER_FOR_CONTINUE;
	cin.get();
	cin.get();
}

/**
 * init entities function which reads from file data.txt
 * executes only while starting application
 */
void init_entities() {
	ifstream in("data.txt");
	string line;

	if (in.is_open()) {
		while (getline(in, line)) {
			vector<string> data = common_util::split(line, " ");

			struct train_ticket ticket{};

			std::strcpy(ticket.fio, data[0].c_str());
			std::strcpy(ticket.point_from, data[1].c_str());
			std::strcpy(ticket.point_to, data[2].c_str());
			ticket.price = stod(data[3]);

			train_tickets.push_back(ticket);
		}
	}
}

/**
 * save ticket data to file data.txt
 * works while called #add_ticket()
 * @param ticket - saved structure
 */
void save_to_file(struct train_ticket ticket) {
	fstream file(FILE_PATH, ios::app);

	if (file.is_open()) {
		file << strcat(ticket.fio, " ") <<
		     strcat(ticket.point_from, " ") <<
		     strcat(ticket.point_to, " ") <<
		     ticket.price << output_util::NEW_LINE;
	}
}

/**
 * update entity counter method
 */
static void update_counter() {
	int current_count = 0;

	for (const auto & ticket : train_tickets) {
		if (ticket.price > 0) {
			current_count++;
		}
	}

	if (current_count == entity_counter) {
		return;
	}

	entity_counter = current_count;
}

// below is the main functionality for laboratory work

void print_tickets() {
	cout << output_util::PRINT_TICKETS;

	for (const auto & ticket : train_tickets) {
		if (ticket.price > 0) {
			output_util::print_ticket(ticket);
		}
	}

	cout << output_util::LINE;
	cout << output_util::NEW_LINE << output_util::COUNT_ENTITIES << entity_counter;
	wait_for_enter();
}

void add_ticket() {
	struct train_ticket ticket{};

	input_util::print_fio();
	cin >> ticket.fio;

	input_util::print_point_from();
	cin >> ticket.point_from;

	input_util::print_point_to();
	cin >> ticket.point_to;

	input_util::print_price();
	cin >> ticket.price;

	train_tickets.push_back(ticket);
	save_to_file(ticket);

	wait_for_enter();
}

void find_ticket() {
	char fio[40];

	cout << input_util::FIND_TICKET;
	cin >> fio;

	for (const auto & ticket : train_tickets) {
		if (::strcmp(fio, ticket.fio) == 0) {
			output_util::print_ticket(ticket);
			wait_for_enter();

			return;
		}
	}

	output_util::print_empty();
	wait_for_enter();
}

/**
 * multiply function which can sort asc and desc by price value
 * @param asc
 */
void sort(sort_type type) {
	switch (type) {
		case asc_price:
			sort(train_tickets.begin(), train_tickets.end(), greater<>());
			break;
		case desc_price:
			sort(train_tickets.begin(), train_tickets.end(), less<>());
			break;
		case asc_fio:
			sort(train_tickets.begin(), train_tickets.end(), fio_comparator_asc());
			break;
		case desc_fio:
			sort(train_tickets.begin(), train_tickets.end(), fio_comparator_desc());
			break;
	}

	print_tickets();
}

static void menu() {
	init_entities();

	while(true){
		output_util::print_menu();

		int choice;
		cin >> choice;

		update_counter();

		switch (choice) {
			case 1:
				print_tickets();
				break;
			case 2:
				find_ticket();
				break;
			case 3:
				add_ticket();
				break;
			case 4:
				sort(desc_price);
				break;
			case 5:
				sort(asc_price);
				break;
			case 6:
				sort(desc_fio);
				break;
			case 7:
				sort(asc_fio);
				break;
			case 8:
				::exit(1);
			default:
				cout<< output_util::COMMAND_NOT_FOUND;
				break;
		}

	}
}

int main() {
	menu();

    return 0;
}
