#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <limits>

#ifdef max	// Undefine max macro (likely from an automatic Windows header)
#undef max
#endif

//// Headers for customizing console window
//#ifdef _WIN32
//#include <windows.h>
//#endif

int getIntegerPostID(int postID);		// Returns validated integer postID
int getIntegerUserID(int userID);		// Returns validated integer userID
void fetchAndDisplayPost(int postID);


int main() {
//// Code to customize console window to 100 characters in width - DOES NOT SEEM TO WORK
//#ifdef _WIN32
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	if (hConsole == INVALID_HANDLE_VALUE) {
//		std::cerr << "Error: Could not get console handle.\n";
//		return 1;
//	}
//
//	// Attempt to set the window size FIRST
//	SMALL_RECT windowSize = { 0, 0, 2, 36 }; // Desired window: 100 width, 25 height (0-indexed)
//	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize)) {
//		std::cerr << "Error: Could not set console window info. Error code: " << GetLastError() << "\n";
//	}
//
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
//		std::cout << "Current buffer size: Width=" << csbi.dwSize.X << ", Height=" << csbi.dwSize.Y << std::endl;
//		std::cout << "Current window: Left=" << csbi.srWindow.Left << ", Top=" << csbi.srWindow.Top
//			<< ", Right=" << csbi.srWindow.Right << ", Bottom=" << csbi.srWindow.Bottom << std::endl;
//
//		COORD newBufferSize = { 100, static_cast<SHORT>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1 + 20) }; // Buffer height based on the *new* window
//		if (!SetConsoleScreenBufferSize(hConsole, newBufferSize)) {
//			std::cerr << "Error: Could not set console buffer size. Error code: " << GetLastError() << "\n";
//		}
//	}
//	else {
//		std::cerr << "Error: Could not get console screen buffer info.\n";
//		return 1;
//	}
//#else
//	std::cout << "\e[8;24;100t"; // Attempt to set size on non-Windows
//#endif


	// Program code begins here
	int menuChoice = -1;
	int userID = -1;
	int postID = -1;
	std::string postTitle = "";
	std::string postBody = "";
	cpr::Response response;

	std::cout << "Welcome to Celia's Post Manager!\n"
		<< "This program demonstrates how applications manage online data by communicating with web "
		<< "interfaces. It connects to the public JSON API JSONPlaceholder (https://jsonplaceholder.typicode.com/)"
		<< " and uses standard web protocols like HTTP to simulate basic Create, Read, Update, and Delete"
		<< " (CRUD) operations on sample online posts.\n"
		<< "*N.B.: Because JSONPlaceholder is a dummy API, changes will not actually be saved. Use your "
		<< "imagination! :)\n\n";

	while (true) {	// Loop to repeat program until user selects 5 (Exit) choice
		std::cout << "What would you like to do? \n"
			<< "\t1. Read a Post\n"
			<< "\t2. Add a new Post\n"
			<< "\t3. Update a Post\n"
			<< "\t4. Delete a Post\n"
			<< "\t5. Exit\n";
			
		// Prompt user for menu choice
		std::cout << "> Enter a number for your choice: ";
		while (true) {	// Loop to validate menu choice
			std::string menuChoiceLine;
			std::getline(std::cin, menuChoiceLine);	// Read the entire input line

			// Validate for empty or whitespace input
			bool is_whitespace = true;	// Default setting
			for (char ch : menuChoiceLine) {	// Assign each char in menuChoiceLine to ch then...
				if (!std::isspace(ch)) {	// If ch is not whitespace
					is_whitespace = false;
					break;	// Exit whitespace check loop when a non-space is found
				}
			}
			if (is_whitespace || menuChoiceLine.empty()) {	// If menuChoiceLine is only whitespace or empty
				std::cout << "ERROR: Empty or whitespace input.\n"
					<< "> Enter a whole number between 1 and 5: ";
				continue;	// Go back to beginning of menu choice inner loop & wait for input again (Go to next iteration to prompt again)
			}

			// Try to convert non-whitespace/-empty input to an int
			try {
				size_t pos = 0;	// var to store position of last ch parsed by std::stoi
				menuChoice = std::stoi(menuChoiceLine, &pos); // Try to convert line to an int & save index of 1st nonconverted char to "pos" var
				if (pos != menuChoiceLine.length()) {	// If there are trailing chs after the int (e.g. with input like "2.5", "4dc", "3 is mt choice"), i.e. the entire line was not converted to an int 
					throw std::invalid_argument("ERROR: Non-integer input.");
				}
				if (menuChoice >= 1 && menuChoice <= 5) {	// If menuChoice is valid (an int between 1 and 5)
					break;	// Exit menu choice validation loop & go to switch statement
				} else {	// If menuChoice is an int but not between 1 and 5
					std::cout << "ERROR: Invalid integer.\n"
						<< "> Enter a whole number between 1 and 5: ";
					continue; // Go back to beginning of menu choice validation loop
				}
			} catch (const std::invalid_argument& e) {	// If menuChoiceLine cannot be converted to an int or has trailing non-int chs
				//std::cout << "ERROR: " << e.what() << ".\n"	// Print the error message from the exception
				//	<< "> Enter a whole number between 1 and 5: ";
				std::cout << "ERROR: Non-integer input.\n"	// CUSTOMIZED
					<< "> Enter a whole number between 1 and 5: ";
			} catch (const std::out_of_range& e) {	// If converted int is outside the representable range of an int
				std::cout << "ERROR: Input out of range.\n"
					<< "> Enter a valid integer: ";
			}
		}

		// Once menuChoice is a valid integer
		switch (menuChoice) {
			case 1: // Read a Post
			{
				std::cout << "\nOK, let's read a post.\n"
					<< "> Enter the ID for the Post you want to read: ";
				postID = getIntegerPostID(postID);	// Get and store valid post ID

				// Check if postID already exists by trying to fetch it
				std::cout << "Checking if Post ID " << postID << " exists...\n";
				fetchAndDisplayPost(postID);
				break;
			}
			case 2: // Add a Post
			{
				bool postIDExists = false;	// Default assumption is postID does not exist
				
				std::cout << "\nOK, let's add a post.\n"
					<< "> Enter an ID for the Post you want to add: ";
				while (!postIDExists) {	// Loop to check that postID can be added. While postID does not exist...
					postID = getIntegerPostID(postID);	// Get a valid integer postID

					// Check if postID already exists by trying to fetch it
					std::cout << "Checking if Post ID " << postID << " exists...\n";
					auto response = cpr::Get(cpr::Url{"https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID)});

					if (response.status_code == 200) {	// If fetch request is successful
						std::cout << "ERROR: Cannot create Post ID " << postID << " because it already exists (Status Code " << response.status_code << ").\n"
							<< "> Enter a non-assigned ID for the Post you want to add: ";
					} else if (response.status_code == 404) {	// If postID is not found
						postIDExists = true;	// PostID doesn't already exist & addition can proceed
						std::cout << "Post ID " << postID << " does not exist. Let's create your Post.\n";
					} else {
						std::cout << "ERROR: Error checking if Post ID exists (Status Code " << response.status_code << ").\n"
							<< "> Please try again. Enter an ID for the Post you want to add: ";
					}
				}
				
				// Once program confirms postID does not already exist, get input for new post
				std::cout << "> Enter Post title: ";
				std::getline(std::cin, postTitle);	// Read input line (incl. spaces) until \n, discard \n, & save to postTitle
				std::cout << "> Enter Post body: ";
				std::getline(std::cin, postBody);
				std::cout << "> Enter your user ID: ";
				userID = getIntegerUserID(userID);	// Get a valid integer userID
				std::cout << "\n";
				////////// TEST if userID/postID holds on to previous

				std::cout << "Creating Post ID " << postID << " with your input..." << std::endl;
				response = cpr::Post(
					cpr::Url{ "https://jsonplaceholder.typicode.com/posts" },
					cpr::Payload{ {"title", postTitle}, {"body", postBody}, {"userId", std::to_string(userID)} });

				std::cout << "INFO: Post ID " << postID << " created successfully (Status Code " << response.status_code << "):\n";
				std::cout << response.text << "\n\n";	// Display the Post
				break;
			}
			case 3: // Update a Post
			{
				bool postIDExists = false;	// Default assumption is postID does not exist

				std::cout << "\nOK, let's update a post.\n"
					<< "> Enter the ID of the Post you want to update: ";
				while (!postIDExists) {	// Loop to get valid postID. While postID does not exist...
					postID = getIntegerPostID(postID);	// Get validated integer postID
				
					// Check if postID exists by trying to fetch it
					std::cout << "Checking if Post ID " << postID << " exists...\n";
					response = cpr::Get(cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) });

					if (response.status_code == 404) {	// If postID is not found
						std::cout << "ERROR: Cannot update Post ID " << postID << " because it does not exist (Status Code " << response.status_code << ").\n"
							<< "> Enter an existing Post ID to update: ";
					}
					else if (response.status_code == 200) {	// If fetch request is successful
						postIDExists = true;	// PostID exists & program can proceed
					}
					else {
						std::cout << "ERROR: Error checking if Post ID exists (Status Code " << response.status_code << ").\n"
							<< "> Please try again. Enter an existing Post ID to update: ";
					}
				}

				// Once program confirms postID exists
				std::cout << "Post ID " << postID << " exists. Would you like to view the post before you update?\n"
					<< "> Enter \"y\" for \"Yes\" or \"n\" for \"No\": ";
				while (true) {	// Loop to get valid viewPostChoice
					std::string viewPostChoiceLine;
					std::getline(std::cin, viewPostChoiceLine);
					if (!viewPostChoiceLine.empty()) {
						if (viewPostChoiceLine == "y" || viewPostChoiceLine == "Y") {   // Show existing post
							fetchAndDisplayPost(postID);
							break;	// Exit loop and continue
						}
						else if (viewPostChoiceLine == "n" || viewPostChoiceLine == "N") {    // Do not show existing post
							std::cout << "Alright, living on the edge!\n";
							break;
						}
						else {	// Loop continues to prompt user again
							std::cout << "ERROR: Invalid response.\n"
								<< "> Enter \"y\" for \"Yes\" or \"n\" for \"No\": ";
						}
					} else {
						std::cout << "ERROR: Empty input.\n"
							<< "> Enter \"y\" for \"Yes\" or \"n\" for \"No\": ";
					}
				}

				// Get input for updated post
				std::cout << "Let's enter new content for this post.\n";
				std::cout << "> Enter new Post title: ";
				std::getline(std::cin, postTitle);
				std::cout << "> Enter new Post body: ";
				std::getline(std::cin, postBody);
				std::cout << "> Enter your user ID: ";
				userID = getIntegerUserID(userID);	// Get a valid integer userID

				std::cout << "Updating Post ID " << postID << " with your input...\n";
				response = cpr::Put(
					cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) },
					cpr::Payload{ {"title", postTitle}, {"body", postBody}, {"userId", std::to_string(userID)} });

				std::cout << "INFO: Post ID " << postID << " updated successfully (Status Code " << response.status_code << "):\n";
				std::cout << response.text << "\n\n";
				break;
			}
			case 4:	// Delete a Post
			{
				bool postIDExists = false;	
				
				std::cout << "\nOK, let's delete a post.\n"
					<< "> Enter the ID of the Post you want to delete: ";
				while (!postIDExists) {	// Loop to check that postID can be deleted. While postID does not exist...
					postID = getIntegerPostID(postID);
					
					// Check if postID exists by trying to fetch it
					std::cout << "Checking if Post ID " << postID << " exists...\n";
					auto response = cpr::Get(cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) });

					if (response.status_code == 404) {	// If postID is not found
						std::cout << "ERROR: Cannot delete Post ID " << postID << " because it does not exist (Status Code " << response.status_code << ").\n\n";
						break;
					} else if (response.status_code == 200) {	// If fetch request is successful
						postIDExists = true;	// PostID exists & deletion can proceed
						std::cout << "Post ID " << postID << " exists. Now deleting...\n";
						response = cpr::Delete(cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) });
						std::cout << "INFO: Post deleted successfully (Status Code " << response.status_code << ").\n\n";
					} else {
						std::cout << "ERROR: Error checking if Post ID exists (Status Code " << response.status_code << ").\n"
							<< "> Please try again. Enter an ID for the Post you want to delete: ";
					}
				}

				// Once program confirms postID exists
				/*response = cpr::Delete(cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) });
				std::cout << "INFO: Post deleted successfully (Status Code " << response.status_code << ").\n\n";*/
				break;
			}
			case 5:	// Exit
			{
				std::cout << "\nOK, now exiting Program Manager. Come back soon!\n";
				return 0;
				break;
			}
			default: // User choice doesn't match any case
			{
				std::cout << "ERROR: Invalid choice. Try again.\n";
			}
		}
	}

	return 0;
}



int getIntegerPostID(int postID) {	// Returns validated integer postID
	std::string postIDLine;
	while (true) {
		std::getline(std::cin, postIDLine);
		//std::cout << "\n";

		// Validate for empty or whitespace input
		bool is_whitespace = true;
		for (char ch : postIDLine) {
			if (!std::isspace(ch)) {
				is_whitespace = false;
				break;
			}
		}
		if (is_whitespace || postIDLine.empty()) {
			std::cout << "ERROR: Empty or whitespace input.\n"
				<< "> Enter a whole number for Post ID: ";
			continue;
		}

		// Try to convert non-whitespace/-empty input to an int
		try {
			size_t pos = 0;
			postID = std::stoi(postIDLine, &pos);
			if (pos != postIDLine.length()) {
				throw std::invalid_argument("ERROR: Non-integer input.");
			}
			break; // Exit loop if input is a valid integer
		} catch (const std::invalid_argument& e) {
			std::cout << "ERROR: Non-integer input.\n"
				<< "> Enter a whole number for Post ID: ";
		} catch (const std::out_of_range& e) {
			std::cout << "ERROR: Input out of range.\n"
				<< "> Enter a valid integer for Post ID: ";
		}
	}
	return postID;	// Return validated integer postID
}

int getIntegerUserID(int userID) {	// Returns validated integer userID
	std::string userIDLine;
	while (true) {
		std::getline(std::cin, userIDLine);

		// Validate for empty or whitespace input
		bool is_whitespace = true;
		for (char ch : userIDLine) {
			if (!std::isspace(ch)) {
				is_whitespace = false;
				break;
			}
		}
		if (is_whitespace || userIDLine.empty()) {
			std::cout << "ERROR: Empty or whitespace input.\n"
				<< "> Enter a whole number for user ID: ";
			continue;
		}

		// Try to convert non-whitespace/-empty input to an int
		try {
			size_t pos = 0;
			userID = std::stoi(userIDLine, &pos);
			if (pos != userIDLine.length()) {
				throw std::invalid_argument("ERROR: Non-integer input.");
			}
			break; // Exit loop if input is a valid integer
		} catch (const std::invalid_argument& e) {
			std::cout << "ERROR: Non-integer input.\n"
			<< "> Enter a whole number for user ID: ";
		} catch (const std::out_of_range& e) {
			std::cout << "ERROR: Input out of range.\n"
			<< "> Enter a valid integer for user ID: ";
		}
	}
	return userID;	// Return validated integer postID
}

void fetchAndDisplayPost(int postID) {
	auto response = cpr::Get(cpr::Url{ "https://jsonplaceholder.typicode.com/posts/" + std::to_string(postID) });
	
	if (response.status_code == 200) {	// If post fetch request is successful
		std::cout << "Now fetching Post ID " << postID << "...\n";
		std::cout << "INFO: Post ID " << postID << " fetched successfully (Status Code " << response.status_code << "):\n";
		std::cout << response.text << "\n\n";	// Display the Post
	} else if (response.status_code == 404) {	// If postID is not found
		std::cout << "ERROR: Post ID " << postID << " not found (Status Code " << response.status_code << ").\n"
			<< "> Enter the ID for the Post you want to read: \n";
	} else {
		std::cout << "ERROR: Error fetching Post ID " << postID << " (Status Code " << response.status_code << ").\n"
			<< "> Please try again. Enter the ID for the Post you want to read: \n";
	}
}