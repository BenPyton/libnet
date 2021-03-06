// Network API Example: Simple Chat Client

#include <Network.h>
#include <regex> // to check IP format

using namespace std;

bool connected = false;

void OnMessageReceived(net::Socket* _sock, string _msg)
{
	// Simply print the received message
	printf("%s\n", _msg.c_str());
}

void OnConnection(net::Socket* _server)
{
	printf("Connected to the server!\n\n\n");
}

void OnDisconnection(net::Socket* _server)
{
	printf("Lost connection with the server...\n");
	connected = false;
}

// Ask the user to enter a value of a certain type
template<typename T> T Ask(string question)
{
	T value = 0;
	bool good = false;

	// We loop until the user enter a number
	do
	{
		// We ask the question and get the user input
		cout << question;
		cin >> value;

		// we clear cin when a number is not entered
		good = !cin.fail();
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	} while (!good);

	return value;
}
string Ask(string question)
{
	string value;
	bool good = false;

	// We loop until the user enter a number
	do
	{
		// We ask the question and get the user input
		cout << question;
		getline(cin, value);

		// we clear cin when a number is not entered
		good = !cin.fail();
	} while (!good);

	return value;
}
string AskIP(string question)
{
	string value;
	bool good = false;

	regex r("[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}");

	// We loop until the user enter a number
	do
	{
		// We ask the question and get the user input
		cout << question;
		getline(cin, value);

		good = regex_match(value, r);
	} while (!good);

	return value;// wstring(value.begin(), value.end());
}

int main()
{
	net::Socket::Init();

	net::Client client;

	client.onConnection().subscribe(OnConnection);
	client.onDisconnection().subscribe(OnDisconnection);
	client.onMessageReceived().subscribe(OnMessageReceived);
	string username = Ask("username: ");

	do // Try to connect to the server
	{
		string addr = AskIP("Enter server address: ");
		UINT16 port = Ask<UINT16>("Enter server port: ");

		try
		{
			client.connect(addr, port);
			connected = true;
		}
		catch (net::SocketException _e)
		{
			cout << "Can't connect to " << addr << ":" << port << endl;
			connected = false;
		}
	} while (!connected); 

	string message;
	while (client.isRunning()) // main loop
	{
		getline(cin, message);

		// An empty string is entered or connection is lost, then disconnect the client properly
		if (message.empty() || !connected)
		{
			client.disconnect();
		}
		else // send the message to the server
		{
			client.send("[" + username + "] " + message);
		}
	}

	net::Socket::Quit();

	cout << "Program Terminated." << endl;

	system("pause");
    return 0;
}
