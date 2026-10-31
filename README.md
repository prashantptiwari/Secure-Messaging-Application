Secure Messaging App

A console-based **Secure Messaging Application developed in C++** that allows users to register, log in, communicate with other registered users, manage contacts, and track message delivery and seen status.

The application uses file handling for data storage and XOR-based transformation for password obfuscation.

Project Overview

The Secure Messaging App is designed to provide a simple platform for user-to-user communication through a command-line interface. It supports account management, private messaging, chat history, contact management, and online user tracking.

The application stores user information, messages, and contacts in separate text files, allowing data to remain available after restarting the program.

Features

- **User Registration:** Create a new account with a unique username and password.
- **User Login:** Authenticate users using their registered credentials.
- **Password Recovery:** Reset an existing account password.
- **Send Messages:** Send private messages to other registered users.
- **View Messages:** View received messages with sender details and timestamps.
- **Message Status:** Track delivered and seen message status.
- **Delete Messages:** Delete sent messages from the application's view.
- **Online User Tracking:** Display currently logged-in users and their session duration.
- **Chat System:** Chat with other registered users and view previous conversations.
- **Contact Management:** Add contacts, view contact lists, and manage communication connections.
- **Session Tracking:** Calculate and display the duration of each login session.
- **Persistent Storage:** Save user accounts, messages, and contacts using text files.

Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core programming language |
| OOP | Classes, objects, and encapsulation |
| File Handling | Persistent data storage |
| STL | Vector, Map, and Set |
| XOR Operation | Basic password obfuscation |
| Time Library | Message timestamps and session tracking |

Project Structure

```text
Secure-Messaging-App/
│
├── main.cpp          # Main application source code
├── users.txt         # Registered user information
├── messages.txt      # Stored message records
├── contacts.txt      # Saved user contacts
│
└── README.md         # Project documentation
```

Note: The text files are generated automatically when the application saves data.

Installation and Setup

1. Clone the Repository

```bash
git clone (https://github.com/prashantptiwari/Secure-Messaging-Application.git)

2. Open the Project Folder

```bash
cd Secure-Messaging-App
```
Compile the Program

Using the GCC C++ compiler:

```bash
g++ main.cpp -o messaging
```
Run the Application

Windows:

```bash
messaging.exe
```

For Linux/macOS:

```bash
./messaging
```

How to Use

Step 1: Start the Application

After running the program, the main menu appears:

1. Register
2. Login
3. Forget Password
4. Exit

Step 2: Register an Account

- Select the Register option.
- Enter a unique username.
- Create a password.
- Your account details will be saved.

Step 3: Login

- Enter your registered username and password.
- After successful authentication, the user dashboard will appear.

Step 4: Access Messaging Features

After logging in, you can:

1. Send Message
2. View Messages
3. Delete Message
4. Show Online Users
5. Chat with User
6. Show My Contacts
7. Logout
8. Add Contact
9. View Contact List

Step 5: Logout

Select the Logout option to end your session and view your session duration.

Data Storage

The application uses three text files for storing information:

- **users.txt:** Stores registered usernames and XOR-obfuscated passwords.
- **messages.txt:** Stores message records, sender and receiver information, timestamps, and message status.
- **contacts.txt:** Stores contact information associated with users.

This file-based approach allows the application to retain information between executions.

Security Information

The project implements a basic XOR-based transformation for password obfuscation.

**Important:** XOR is not modern encryption and does not provide strong security. This project is intended for learning and demonstrating C++ programming concepts, not for protecting real private communications. The current message storage implementation also does not encrypt message content at rest.

Learning Outcomes

Through this project, the following programming concepts are demonstrated:

- Object-Oriented Programming in C++.
- File handling and data persistence.
- User authentication and account management.
- Data structures using STL.
- String manipulation and XOR operations.
- Date and time handling.
- Implementation of a menu-driven application.
- Managing communication between multiple users.

Future Enhancements

- Implement strong password hashing.
- Add modern end-to-end message encryption.
- Develop a graphical user interface.
- Introduce real-time network-based communication.
- Add group messaging functionality.
- Implement secure user authentication.
- Add profile management and profile pictures.

Author

Prashant Tiwari

---

⭐ If you find this project useful, consider giving the repository a star!
