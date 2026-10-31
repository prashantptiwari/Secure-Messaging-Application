#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <set>
#include <cstdlib>

using namespace std;

string xorEncryptDecrypt(const string &text, char key = 'K')
{
    string result = text;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] ^= key;
    return result;
}

struct Message
{
    string sender;
    string receiver;
    string content;
    string timestamp;
    bool seen;
    bool deleted;

    Message(string s, string r, string c, string timeStr = "", bool seenStatus = false, bool del = false)
        : sender(s), receiver(r), seen(seenStatus), deleted(del)
    {
        content = xorEncryptDecrypt(c);
        if (timeStr.empty())
        {
            time_t now = time(0);
            timestamp = string(ctime(&now));
            if (!timestamp.empty() && timestamp[timestamp.size() - 1] == '\n')
                timestamp.erase(timestamp.size() - 1);
        }
        else
        {
            timestamp = timeStr;
        }
    }

    string format() const
    {
        return sender + "|" + receiver + "|" + xorEncryptDecrypt(content) + "|" + timestamp + "|" + (seen ? "1" : "0") + "|" + (deleted ? "1" : "0");
    }
};

class User
{
public:
    string username;
    string encryptedPassword;
    time_t loginTime;

    User() : loginTime(0) {}
    User(string uname, string pass) : username(uname), loginTime(0)
    {
        encryptedPassword = xorEncryptDecrypt(pass);
    }

    bool authenticate(const string &u, const string &p) const
    {
        return username == u && encryptedPassword == xorEncryptDecrypt(p);
    }
};

class MessagingApp
{
private:
    vector<User> users;
    vector<Message> messages;
    string currentUser;
    map<string, time_t> onlineUsers;
    map<string, set<string>> contacts;

    bool userExists(const string &uname) const
    {
        for (size_t i = 0; i < users.size(); ++i)
            if (users[i].username == uname)
                return true;
        return false;
    }

    int getUserIndex(const string &uname) const
    {
        for (size_t i = 0; i < users.size(); ++i)
            if (users[i].username == uname)
                return static_cast<int>(i);
        return -1;
    }

    void saveUsers()
    {
        ofstream out("users.txt");
        for (size_t i = 0; i < users.size(); ++i)
            out << users[i].username << "|" << users[i].encryptedPassword << "\n";
        out.close();
    }

    void loadUsers()
    {
        ifstream in("users.txt");
        string line;
        while (getline(in, line))
        {
            size_t pos = line.find("|");
            if (pos != string::npos)
            {
                string uname = line.substr(0, pos);
                string pass = xorEncryptDecrypt(line.substr(pos + 1));
                users.push_back(User(uname, pass));
            }
        }
        in.close();
    }

    void saveMessages()
    {
        ofstream out("messages.txt");
        for (size_t i = 0; i < messages.size(); ++i)
            out << messages[i].format() << "\n";
        out.close();
    }

    void loadMessages()
    {
        ifstream in("messages.txt");
        string line;
        while (getline(in, line))
        {
            size_t p1 = line.find("|");
            size_t p2 = line.find("|", p1 + 1);
            size_t p3 = line.find("|", p2 + 1);
            size_t p4 = line.find("|", p3 + 1);
            size_t p5 = line.find("|", p4 + 1);
            if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos || p5 == string::npos)
                continue;

            string s = line.substr(0, p1);
            string r = line.substr(p1 + 1, p2 - p1 - 1);
            string c = line.substr(p2 + 1, p3 - p2 - 1);
            string t = line.substr(p3 + 1, p4 - p3 - 1);
            bool seen = (line.substr(p4 + 1, p5 - p4 - 1) == "1");
            bool del = (line.substr(p5 + 1) == "1");

            messages.push_back(Message(s, r, c, t, seen, del));
        }
        in.close();
    }

    void saveContacts()
    {
        ofstream out("contacts.txt");
        for (map<string, set<string>>::iterator it = contacts.begin(); it != contacts.end(); ++it)
        {
            out << it->first;
            for (set<string>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
                out << "|" << *sit;
            out << "\n";
        }
        out.close();
    }

    void loadContacts()
    {
        ifstream in("contacts.txt");
        string line;
        while (getline(in, line))
        {
            size_t pos = line.find("|");
            if (pos != string::npos)
            {
                string user = line.substr(0, pos);
                set<string> userContacts;
                size_t start = pos + 1;
                while ((pos = line.find("|", start)) != string::npos)
                {
                    userContacts.insert(line.substr(start, pos - start));
                    start = pos + 1;
                }
                userContacts.insert(line.substr(start));
                contacts[user] = userContacts;
            }
        }
        in.close();
    }

    void showUserStats()
    {
        cout << "Total registered users: " << users.size() << "\n";
        if (contacts.find(currentUser) != contacts.end())
            cout << "You have " << contacts[currentUser].size() << " contact(s) in your list.\n";
        else
            cout << "You have 0 contacts in your list.\n";
    }

public:
    void registerUser()
    {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        if (userExists(uname))
        {
            cout << "User already exists.\n";
            return;
        }
        cout << "Enter password: ";
        cin >> pass;
        users.push_back(User(uname, pass));
        saveUsers();
        cout << "Registration successful.\n";
    }

    bool loginUser()
    {
        string uname, pass;
        cout << "Enter username: ";
        cin >> uname;
        cout << "Enter password: ";
        cin >> pass;
        for (size_t i = 0; i < users.size(); ++i)
        {
            if (users[i].authenticate(uname, pass))
            {
                currentUser = uname;
                users[i].loginTime = time(0);
                onlineUsers[uname] = users[i].loginTime;
                cout << "Login successful.\n";
                showUserStats(); // New Feature
                return true;
            }
        }
        cout << "Invalid credentials.\n";
        return false;
    }

    void logoutUser()
    {
        if (currentUser.empty())
            return;
        time_t now = time(0);
        double duration = difftime(now, onlineUsers[currentUser]);
        cout << "Session duration: " << static_cast<int>(duration) << " seconds.\n";
        onlineUsers.erase(currentUser);
        currentUser.clear();
    }

    void sendMessage()
    {
        string to, msg;
        cout << "Send to: ";
        cin >> to;
        if (!userExists(to))
        {
            cout << "User not found.\n";
            return;
        }
        cin.ignore();
        cout << "Enter message: ";
        getline(cin, msg);
        messages.push_back(Message(currentUser, to, msg));
        saveMessages();
        cout << "Message sent.\n";
    }

    void viewMessages()
    {
        bool found = false;
        cout << "\n=== Your Messages ===\n";
        for (size_t i = 0; i < messages.size(); ++i)
        {
            if (messages[i].receiver == currentUser && !messages[i].deleted)
            {
                cout << "From: " << messages[i].sender
                     << "\nMessage: " << xorEncryptDecrypt(messages[i].content)
                     << "\nTime: " << messages[i].timestamp
                     << (messages[i].seen ? "\n✓✓ Seen\n\n" : "\n✓ Delivered\n\n");
                messages[i].seen = true;
                found = true;
            }
        }
        if (!found)
            cout << "No new messages.\n";
        saveMessages();
    }

    void deleteMessage()
    {
        int idx = 0;
        vector<int> indices;
        cout << "\n=== Your Sent Messages ===\n";
        for (size_t i = 0; i < messages.size(); ++i)
        {
            if (messages[i].sender == currentUser && !messages[i].deleted)
            {
                cout << ++idx << ". To: " << messages[i].receiver
                     << " - " << xorEncryptDecrypt(messages[i].content)
                     << (messages[i].seen ? " ✓✓" : " ✓") << "\n";
                indices.push_back(static_cast<int>(i));
            }
        }
        if (idx == 0)
        {
            cout << "No messages to delete.\n";
            return;
        }
        int choice;
        cout << "Enter message number to delete: ";
        cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(indices.size()))
        {
            messages[indices[choice - 1]].deleted = true;
            cout << "Message deleted.\n";
            saveMessages();
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    void showOnlineUsers()
    {
        cout << "\n=== Online Users ===\n";
        if (onlineUsers.empty())
        {
            cout << "No users online.\n";
            return;
        }
        for (map<string, time_t>::iterator it = onlineUsers.begin(); it != onlineUsers.end(); ++it)
        {
            double dur = difftime(time(0), it->second);
            cout << it->first << " (" << static_cast<int>(dur) << " seconds online)\n";
        }
    }

    void showMyContacts()
    {
        set<string> chatContacts;
        for (size_t i = 0; i < messages.size(); ++i)
        {
            if (messages[i].deleted)
                continue;
            if (messages[i].sender == currentUser && messages[i].receiver != currentUser)
                chatContacts.insert(messages[i].receiver);
            else if (messages[i].receiver == currentUser && messages[i].sender != currentUser)
                chatContacts.insert(messages[i].sender);
        }
        if (chatContacts.empty())
        {
            cout << "\nNo chat contacts yet.\n";
            return;
        }
        cout << "\n=== My Contacts (From Chats) ===\n";
        int count = 1;
        for (set<string>::iterator it = chatContacts.begin(); it != chatContacts.end(); ++it)
        {
            cout << count++ << ". " << *it << "\n";
        }
    }

    void addContact()
    {
        string contact;
        cout << "Enter username to add as contact: ";
        cin >> contact;
        if (!userExists(contact))
        {
            cout << "User does not exist.\n";
            return;
        }
        if (contact == currentUser)
        {
            cout << "You cannot add yourself.\n";
            return;
        }
        contacts[currentUser].insert(contact);
        saveContacts();
        cout << "Contact added successfully.\n";
    }

    void viewMyContactList()
    {
        cout << "\n=== Your Contact List ===\n";
        set<string> &myContacts = contacts[currentUser];
        if (myContacts.empty())
        {
            cout << "No contacts added yet.\n";
            return;
        }
        int i = 1;
        for (set<string>::iterator it = myContacts.begin(); it != myContacts.end(); ++it)
            cout << i++ << ". " << *it << "\n";
    }

    void chatWithUser()
    {
        showMyContacts();
        cout << "\nEnter username to chat with (or type 0 to cancel): ";
        string peer;
        cin >> peer;
        if (peer == "0")
            return;
        if (!userExists(peer))
        {
            cout << "User does not exist.\n";
            return;
        }
        cin.ignore();
        int choice;
        do
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            displayChatHistory(peer);
            cout << "\nOptions:\n1. Send Message\n2. Refresh\n3. Exit Chat\nEnter choice: ";
            cin >> choice;
            cin.ignore();
            if (choice == 1)
            {
                string msg;
                cout << "Enter message: ";
                getline(cin, msg);
                messages.push_back(Message(currentUser, peer, msg));
                saveMessages();
            }
            else if (choice == 2)
            {
                continue;
            }
            else if (choice == 3)
            {
                break;
            }
            else
            {
                cout << "Invalid option.\n";
            }
        } while (true);
    }

    void displayChatHistory(const string &peer)
    {
        cout << "\n--- Chat history with " << peer << " ---\n";
        bool found = false;
        for (size_t i = 0; i < messages.size(); ++i)
        {
            if (messages[i].deleted)
                continue;
            if ((messages[i].sender == currentUser && messages[i].receiver == peer) ||
                (messages[i].sender == peer && messages[i].receiver == currentUser))
            {
                string direction = (messages[i].sender == currentUser) ? "You" : peer;
                cout << direction << " [" << messages[i].timestamp << "]: "
                     << xorEncryptDecrypt(messages[i].content);
                if (messages[i].sender == currentUser)
                {
                    cout << (messages[i].seen ? " ✓✓" : " ✓");
                }
                cout << "\n";
                if (messages[i].receiver == currentUser)
                    messages[i].seen = true;
                found = true;
            }
        }
        if (!found)
            cout << "No chat history found.\n";
        saveMessages();
    }

    void forgetPassword()
    {
        string uname;
        cout << "Enter username for password reset: ";
        cin >> uname;
        int idx = getUserIndex(uname);
        if (idx == -1)
        {
            cout << "User does not exist.\n";
            return;
        }
        string newPass;
        cout << "Enter new password: ";
        cin >> newPass;
        users[idx].encryptedPassword = xorEncryptDecrypt(newPass);
        saveUsers();
        cout << "Password reset successful.\n";
    }

    void userMenu()
    {
        int choice;
        do
        {
            cout << "\n1. Send Message\n2. View Messages\n3. Delete Message\n4. Show Online Users\n5. Chat with User\n6. Show My Contacts\n7. Logout\n8. Add Contact\n9. View Contact List\nEnter choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                sendMessage();
                break;
            case 2:
                viewMessages();
                break;
            case 3:
                deleteMessage();
                break;
            case 4:
                showOnlineUsers();
                break;
            case 5:
                chatWithUser();
                break;
            case 6:
                showMyContacts();
                break;
            case 7:
                logoutUser();
                return;
            case 8:
                addContact();
                break;
            case 9:
                viewMyContactList();
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (true);
    }

    void start()
    {
        loadUsers();
        loadMessages();
        loadContacts();
        int choice;
        do
        {
            cout << "\n--- Secure Messaging App ---\n1. Register\n2. Login\n3. Forget Password\n4. Exit\nEnter choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser())
                    userMenu();
                break;
            case 3:
                forgetPassword();
                break;
            case 4:
                cout << "Goodbye!\n";
                return;
            default:
                cout << "Invalid choice.\n";
            }
        } while (true);
    }
};

int main()
{
    MessagingApp app;
    app.start();
    return 0;
}