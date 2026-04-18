
#include <iostream>
#include <fstream> 
#include <string>   
#include <sstream>
#include <cctype>
#include <limits>
#include <algorithm> 
#include <random>    
#include <chrono>   

using namespace std;

const int MAX_QUESTIONS = 600;

struct Question {
    string text;
    string options[4];
    char correctAnswer;
    int category; 
};

Question allQuestions[MAX_QUESTIONS];
int totalQuestions = 0;
string studentName;
int score = 0;
int attemptedQuestions = 0;

                                                        // FUNCTION PROTOTYPES 
void displayWelcome();
string getStudentName();
int selectCategory();
int loadQuestions(const string &filename, int selectedCategory);
bool validateAnswer(char answer);
int askQuestion(Question question);
void startQuiz();
void showScore();
void saveScore();
void adminPanel();

                                                                //  MAIN 
int main() {
     cout<<"Want to learn basics of C++,but do not know where to start and how to practice?\n";
        cout<<"Do not worry! We have got your back!!\n";
        cout<<"Presenting \"CODE MASTER\"!!!!!\n";
    char choice;
    do {
        cout << "==============================\n";
        cout << "          MAIN MENU           \n";
        cout << "==============================\n";
       
        cout<<"Please select one of the following options.\n";
        cout << "1. Student Portal\n";
        cout << "2. Admin Panel\n";
        cout << "3. Exit Program\n";
        cout << "Select an option (1-3): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {
            case '1': {
                displayWelcome();
                studentName = getStudentName();
                int category = selectCategory();

                totalQuestions = loadQuestions("questions.txt", category);
                if (totalQuestions == 0) {
                    cout << "Sorry there are no questions available for this category now.We are so sorry for this inconvenience.Our team is working on it.\n";
                    break;
                }

                //                  To Shuffle questions so evrytime student gets different questions for each category
                unsigned seed = chrono::system_clock::now().time_since_epoch().count();
                shuffle(allQuestions, allQuestions + totalQuestions, default_random_engine(seed));

                startQuiz();
                showScore();
                saveScore();
                break;
            }

            case '2': {
                adminPanel();
                break;
            }

            case '3': {
                cout << "OKAY SIR,Exiting program. Goodbye!\n";
                cout << "We are still in our learning process, just like you, so please ignore small inconveniences.\n";
                break;
            }

            default:
                cout << "OPSIE!!!Invalid option.Please Try again.\n";
        }

        cout << "\n";
    } while(choice != '3');

    return 0;
}

                                                            //FUNCTIONS 
void displayWelcome() {
    cout << "==============================\n";
    cout << "       WELCOME TO QUIZ        \n";
    cout << "==============================\n";
    cout << "Come on, Batman! It's time for your comeback!\n";
    cout << "You\'re already halfway there just for deciding to practice.\n";
    cout << "Please select which category questions you wanna practice first.\nCategories:\n1. Syntax Errors\n2. Output Prediction\n3. Fill in the Blanks\n4. Mixed\n";
}

string getStudentName() {
    string name;
    cout << "Dear pasionate nerd,please enter your name: ";
    getline(cin, name);
    if (name == "")
        name = "Guest";
    return name;
}

int selectCategory() {
    string categoryStr;
    int categoryChoice = 4; 
    cout << "Select category (1-4): ";
    getline(cin, categoryStr);
    if (!categoryStr.empty() && (categoryStr[0] >= '1' && categoryStr[0] <= '4'))
        categoryChoice = categoryStr[0] - '0';
    else
        cout << "OPSIE!!!Invalid input. To help you we are defaulting to Mixed.\n";

    return categoryChoice;
}


int stringToInt(const string &str) {
    int num = 0;
    for(char ch : str) {
        if(isdigit(ch))
            num = num * 10 + (ch - '0');
        else
            return -1; 
    }
    return num;
}

int loadQuestions(const string &filename, int selectedCategory) {
    ifstream file(filename);
    if(!file) {
        cout << "Cannot open file " << filename << "\n";
        return 0;
    }

    totalQuestions = 0;
    string line;
    while(getline(file, line) && totalQuestions < MAX_QUESTIONS) {
        if(line.empty()) continue;

        stringstream ss(line);
        string categoryStr, questionText, optionA, optionB, optionC, optionD, correctStr;

        if(!getline(ss, categoryStr, '|')) continue;
        if(!getline(ss, questionText, '|')) continue;
        if(!getline(ss, optionA, '|')) continue;
        if(!getline(ss, optionB, '|')) continue;
        if(!getline(ss, optionC, '|')) continue;
        if(!getline(ss, optionD, '|')) continue;
        if(!getline(ss, correctStr)) continue;

        int category = stringToInt(categoryStr);
        if(category == -1) category = 4;

        if(category != selectedCategory && selectedCategory != 4)
            continue;

        allQuestions[totalQuestions].category = category;
        allQuestions[totalQuestions].text = questionText;
        allQuestions[totalQuestions].options[0] = optionA;
        allQuestions[totalQuestions].options[1] = optionB;
        allQuestions[totalQuestions].options[2] = optionC;
        allQuestions[totalQuestions].options[3] = optionD;
        allQuestions[totalQuestions].correctAnswer = toupper(correctStr[0]);

        totalQuestions++;
    }

    file.close();
    return totalQuestions;
}

bool validateAnswer(char answer) {
    answer = toupper(answer);
    return answer == 'A' || answer == 'B' || answer == 'C' || answer == 'D';
}

int askQuestion(Question question) {
    cout << "\n" << question.text << "\n";
    cout << "A) " << question.options[0] << "\n";
    cout << "B) " << question.options[1] << "\n";
    cout << "C) " << question.options[2] << "\n";
    cout << "D) " << question.options[3] << "\n";

    char studentAnswer;
    cout << "Your answer (A/B/C/D or E to exit): ";
    cin >> studentAnswer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    studentAnswer = toupper(studentAnswer);

    if(studentAnswer == 'E') return -1;

    while(!validateAnswer(studentAnswer)) {
        cout << "Invalid input. Enter A/B/C/D: ";
        cin >> studentAnswer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        studentAnswer = toupper(studentAnswer);
    }

    if(studentAnswer == question.correctAnswer) {
        cout << "Correct!\n";
        return 1;
    } else {
        cout << "Wrong! Correct answer: " << question.correctAnswer << "\n";
        return 0;
    }
}

void startQuiz() {
    score = 0;
    attemptedQuestions = 0;

    for(int i = 0; i < totalQuestions; i++) {
        int result = askQuestion(allQuestions[i]);
        if(result == -1) break;
        score += result;
        attemptedQuestions++;
    }
}

void showScore() {
    cout << "\n===== RESULT =====\n";
    cout << "Student: " << studentName << "\n";
    cout << "Score: " << score << "/" << attemptedQuestions << "\n";

    if(attemptedQuestions > 0) {
        float percentage = (score * 100.0) / attemptedQuestions;
        cout << "Percentage: " << percentage << "%\n\n";

        if(percentage >= 90)
            cout << "Congratulations! You're ready to raise the class average! \n";
        else if(percentage >= 80)
            cout << "Woah woah, calm down! If you keep doing this, you'll be called a nerd by classmates \n";
        else if(percentage >= 70)
            cout << "You have potential! Focus on yourself, not the group \n";
        else if(percentage >= 60)
            cout << "Stop doom scrolling and focus on your studies, brother!\n";
        else if(percentage >= 50)
            cout << "You can single-handedly lower the class average! \n";
        else if(percentage >= 40)
            cout << "No comments, sir. Laugh at your skills \n";
        else
            cout << "Opsie! You failed. Now stop complaining and start practicing!\n";
    } else {
        cout << "No questions attempted.\n";
    }
}

void saveScore() {
    ofstream scoreFile("score.txt", ios::app);
    scoreFile << studentName << " => " << score << "/" << attemptedQuestions << "\n";
    scoreFile.close();
}

void adminPanel() {
    string password;
    cout << "Please enter admin password: ";
    getline(cin, password);
    if(password != "admin123") {
        cout << "Yooo you entered wrong password!\n";
        return;
    }

    int adminChoice;
    do {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. Add Questions\n2. View Questions\n3. Delete Question\n4. Exit Admin Panel\n";
        cout << "Select an option: ";
        string choiceStr;
        getline(cin, choiceStr);
        if(!choiceStr.empty())
            adminChoice = choiceStr[0] - '0';
        else
            adminChoice = 0;

        switch(adminChoice) {
            case 1: {                       //this  add Questions in .txt file
                string numStr;
                int numQuestions = 1;
                cout << "How many questions do you want to add? ";
                getline(cin, numStr);
                int temp = stringToInt(numStr);
                if(temp > 0) numQuestions = temp;

                for(int i = 0; i < numQuestions; i++) {
                    string questionText, optionA, optionB, optionC, optionD, categoryStr, correctStr;
                    int category = 4;
                    char correctAnswer = 'A';

                    cout << "\nQuestion " << (i+1) << ":\n";
                    cout << "Enter category (1=Syntax,2=Output,3=Fill,4=Mixed): ";
                    getline(cin, categoryStr);
                    int tempCat = stringToInt(categoryStr);
                    if(tempCat >=1 && tempCat <=4) category = tempCat;

                    cout << "Enter question text: "; getline(cin, questionText);
                    cout << "Option A: "; getline(cin, optionA);
                    cout << "Option B: "; getline(cin, optionB);
                    cout << "Option C: "; getline(cin, optionC);
                    cout << "Option D: "; getline(cin, optionD);

                    cout << "Correct answer (A/B/C/D): "; getline(cin, correctStr);
                    if(!correctStr.empty()) correctAnswer = toupper(correctStr[0]);

                    ofstream questionFile("questions.txt", ios::app);
                    questionFile << category << "|" << questionText << "|" 
                                 << optionA << "|" << optionB << "|" << optionC << "|" << optionD << "|"
                                 << correctAnswer << "\n";
                    questionFile.close();

                    cout << "Question added successfully!\n";
                }
                break;
            }

            case 2: {                      //this helps to  View Questions
                ifstream file("questions.txt");
                string line;
                int count = 1;
                cout << "\n===== ALL QUESTIONS =====\n";
                while(getline(file, line)) {
                    cout << count << ". " << line << "\n";
                    count++;
                }
                file.close();
                break;
            }

            case 3: {                   // this helps to Delete Question
                ifstream file("questions.txt");
                ofstream tempFile("temp.txt");
                string line, delNumStr;
                int delNum;
                cout << "Enter question number to delete: ";
                getline(cin, delNumStr);
                delNum = stringToInt(delNumStr);

                bool deleted = false;
                int count = 1;
                while(getline(file, line)) {
                    if(count != delNum)
                        tempFile << line << "\n";
                    else
                        deleted = true;
                    count++;
                }

                file.close();
                tempFile.close();

                remove("questions.txt");
                rename("temp.txt", "questions.txt");

                if(deleted)
                    cout << "Question deleted successfully!\n";
                else
                    cout << "Invalid question number!\n";

                break;
            }

            case 4:
                cout << "Exiting Admin Panel.\n";
                break;

            default:
                cout << "Invalid option. Try again.\n";
        }
    } while(adminChoice != 4);
}
