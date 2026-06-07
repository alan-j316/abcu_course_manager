/**
 * @file CourseManager.cpp
 * @author Alan Abraham P Kochumon
 * @date Created on: June 07, 2026
 *
 * @brief A digital course planner for ABCU, with ability to load courses from a csv file,
 *        print all courses, and print each course detail.
 *
 * @copyright Copyright (c) 2026 Alan Abraham P Kochumon
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <unordered_set>



/**************************************
 *                                    *
 *         HEADER INTERFACE           *
 *                                    *
 **************************************/

namespace abcu
{
    /**
     * @brief Object representation of a ABCU course.
     */
    class Course
    {
    public:
        // Course Name and id
        std::string name, courseId;
        // Set of all the pre-requisites
        // Set is used here to ensure O(1) access and deletion when removing invalid prerequisites
        std::unordered_set<std::string> prerequisites;

        /**
         * @brief Parse a `std::string` to a course object.
         * @param line The line to parse.
         * @return The @ref Course object.
         *
         * @throws `std::invalid_argument` if the string parsing fail with invalid course name or course id.
         */
        static Course parseCourse(const std::string& line);


        /**
         * @brief Stream @p course to the output stream for printing.
         *
         * @param stream The ostream to stream the object to.
         * @param course The object to stream.
         *
         * @return A reference to @p stream that has the object.
         */
        friend std::ostream& operator<<(std::ostream& stream, const Course& course)
        {
            stream << course.courseId << " (" << course.name << ")\n";
            stream << "Prerequisites: ";

            // Print all the prerequisites in a comma separated form.
            int prerequisiteCount = 0;
            for (const auto& prerequisite : course.prerequisites)
            {
                stream << prerequisite;
                // Only output the delimiter if the current prerequisite is not the last pre-requisite
                if (++prerequisiteCount < course.prerequisites.size())
                    stream << ", ";
            }

            // If there are no pre-requisites, let the user know
            if (prerequisiteCount == 0)
                stream << "None";

            stream << "\n\n";
            return stream;
        }
    };

    /**
     * @brief Single entry in the CourseManager's BST
     */
    class CourseNode
    {
    public:
        Course course;

        // Initialize left and right with nullptr
        CourseNode* left{ nullptr };
        CourseNode* right{ nullptr };
    };

    /**
     * @brief Manager for managing the course data structure. Use BST ADT for storage.
     */
    class CourseManager
    {
        // Initialize root with nullptr
        CourseNode* root{ nullptr };

        /**
         * @brief Remove invalid pre-requisites from the courses.
         *        A prerequisite is considered invalid, if that course id is not present in @p cache.
         *
         * @note To speed up look, the method uses the supplied cache for valid course look,
         *       so providing an empty cache will return the courses without validation.
         *
         * @param node  The root of the course manager.
         * @param cache The cache to use for validation.
         */
        static void removeInvalidPrerequisites(CourseNode* node, const std::unordered_set<std::string>& cache);

        /**
         * @brief Traverse and print the courses in an in-order format.
         * @param node The current root.
         */
        static void printInOrder(const CourseNode* node);

        /**
         * @brief Delete the current course from memory, after freeing its children.
         * @param node
         */
        static void removeAllCourse(CourseNode* node);

    public:
        /**
         * @brief Class destructor.
         */
        ~CourseManager();

        /**
         * @brief Load a CSV file containing courses into a container.
         */
        void loadCourses(const std::string& filename);

        /**
         * @brief Search for a course with an id of @p courseId, and print it's details.
         * @param courseId The courseId to match.
         */
        void printCourse(const std::string& courseId) const;

        /**
         * @brief Insert @ref course into the course manager.
         * @param course The course to insert.
         */
        void insertCourse(const Course& course);

        /**
         * @brief Print the courses in alphabetical sorting.
         */
        void printCourses() const;
    };


    /**
     * @brief Encapsulation of ABCU application with internal parsing and run state.
     */
    class ABCUApplication
    {
    public:
        explicit ABCUApplication(const std::string& filename);

        /**
         * @brief Run the application loop.
         *
         * @note The application maintains its own internal running state, so there is no need to put
         *       run in another application loop.
         */
        void run();

    private:
        bool _running;
        std::string _filename;
        CourseManager _manager;

        /**
         * @brief Parse the user input and perform appropriate action.
         */
        void parseUserInput();

        /**
         * @brief Prints the application menu to screen
         */
        static void printMenu();

        /**
         * @brief Dispatch an action based on the @p actionCode.
         * @param actionCode The code/identifier of the action to perform.
         *
         * @throw `std::invalid_argument` if the @p actionCode is out of range.
         */
        void dispatchAction(int actionCode);
    };

} // namespace abcu



/**************************************
 *                                    *
 *           IMPLEMENTATION           *
 *                                    *
 **************************************/

namespace abcu
{

    namespace utils
    {
        /**
         * @brief Compare if two strings in a case-insensitive manner.
         *
         * @param a The first string to compare.
         * @param b The second string to compare.
         *
         * @return 0 if both are equal, 1 if first string is greater else -1.
         */
        static int strCmp(const std::string& a, const std::string& b)
        {
            const auto charCount = std::min(a.length(), b.length());

            // Compare each character for equality
            // Return false on first character mismatch
            for (std::size_t i = 0; i < charCount; ++i)
            {
                // If the first character of the first string is lexicographically greater return 1
                if (std::tolower(a[i]) > std::tolower(b[i]))
                    return 1;
                // Similarly, when the first character is less than subsequent character is string2, return -1
                else if (std::tolower(a[i]) < std::tolower(b[i]))
                    return -1;
            }

            // Edge case: Either strings are substring of each other
            // If so return 1 if first string is of greater length, else -1 and 0 if both are of same length
            if (a.length() > b.length())
                return 1;
            else if (a.length() < b.length())
                return -1;
            return 0;
        }

    } // namespace utils


    /**************************************
     *                                    *
     *              Course                *
     *                                    *
     **************************************/

    Course Course::parseCourse(const std::string& line)
    {
        std::stringstream ss(line);
        std::string currentToken;
        int iteration = 0;
        Course course;
        while (std::getline(ss, currentToken, ','))
        {
            if (iteration == 0)
            {
                // Course ID
                // If the course name is empty, then it is not a valid course
                if (currentToken.length() < 1)
                    throw std::invalid_argument("[Invalid Course]: Empty Course ID");

                course.courseId = currentToken;
            }
            else if (iteration == 1)
            {
                // Course name
                // If the course name is empty, then it is not a valid course
                if (currentToken.length() < 1)
                    throw std::invalid_argument("[Invalid Course]: Empty Course Name");
                course.name = currentToken;
            }
            else
            {
                // Prerequisites
                if (currentToken.length() > 1)
                    course.prerequisites.insert(currentToken);
            }
            // Increment the iteration count to move forward
            ++iteration;
        }

        return course;
    }

    /**************************************
     *                                    *
     *           Course Manager           *
     *                                    *
     **************************************/

    CourseManager::~CourseManager()
    {
        removeAllCourse(root);
    }

    void CourseManager::loadCourses(const std::string& filename)
    {
        std::cout << "Loading CSV file...\n\n";

        // Cache to add the existing courses so we can filter out any non-existant courses from pre-requisites
        std::unordered_set<std::string> existingCourses;

        // Set the iostream to trigger an exception
        std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

        try
        {
            std::ifstream csvFile(filename);

            std::string line;
            while (std::getline(csvFile, line))
            {
                try
                {
                    auto course = Course::parseCourse(line);
                    // Add the CourseId to the cache
                    existingCourses.insert(course.courseId);
                    insertCourse(course);
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what() << '\n';
                    // Clear the fail bit status
                    std::cin.clear();
                    // Clear the cin buffer
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << '\n';
        }
        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);

        removeInvalidPrerequisites(root, existingCourses);
    }



    void CourseManager::printCourse(const std::string& courseId) const
    {
        if (root == nullptr)
            std::cout << "Empty course list. Consider loading the courses first.\n";

        auto current = root;
        // Print the current course if it's ID matches the provided course id

        while (current != nullptr)
        {
            // If the current course's id and passed-in course id are equal
            // Then print the course information
            if (utils::strCmp(current->course.courseId, courseId) == 0)
            {
                std::cout << current->course;
                return;
            }

            // Else if the search course id lower, search the left subtree
            if (utils::strCmp(courseId, current->course.courseId) < 0)
                current = current->left;
            // else search the right subtree
            else
                current = current->right;
        }

        // If the function doesn't early exit
        // Then, there are no course with the given id
        std::cout << "No course with " << courseId << " found!\n";
    }


    void CourseManager::insertCourse(const Course& course)
    {
        auto node = new CourseNode;
        node->course = course;

        if (root == nullptr)
            root = node;
        else
        {
            auto current = root;
            while (true)
            {
                // If the course to insert has name with lower lexicographical index
                // then move to the left branch
                if (course.courseId < current->course.courseId)
                {
                    if (current->left == nullptr)
                    {
                        // Insert the node at left that slot is empty
                        current->left = node;
                        return; // Return to exit the loop since the node is inserted
                    }
                    // Move forward
                    current = current->left;
                }
                // Else move to the right branch
                else
                {
                    if (current->right == nullptr)
                    {
                        // Insert the node at left that slot is empty
                        current->right = node;
                        return; // Return to exit the loop since the node is inserted
                    }
                    // Move forward
                    current = current->right;
                }
            }
        }
    }


    void CourseManager::printCourses() const
    {
        // Edge case: There are no courses
        if (root == nullptr)
        {
            std::cout << "No courses!";
            return;
        }

        // Helper function that recursively prints the courses
        printInOrder(root);
        std::cout << std::endl;
    }

    void CourseManager::removeInvalidPrerequisites(CourseNode* node, const std::unordered_set<std::string>& cache)
    {
        if (cache.size() < 1)
            return; // No invalidation is performed if the cache is empty

        // Base case
        if (node == nullptr)
            return; // Base case

        // Remove invalid course pre-requisite from the subtrees
        removeInvalidPrerequisites(node->left, cache);
        removeInvalidPrerequisites(node->right, cache);

        for (auto& prerequisite : node->course.prerequisites)
            if (cache.count(prerequisite) == 0)
                node->course.prerequisites.erase(prerequisite);
    }


    void CourseManager::printInOrder(const CourseNode* node)
    {
        if (node == nullptr)
            return;

        printInOrder(node->left);
        std::cout << node->course.courseId << " (" << node->course.name << ")\n";
        printInOrder(node->right);
    }

    void CourseManager::removeAllCourse(CourseNode* node)
    {
        // Base case
        if (node == nullptr)
            return;
        // Recursively delete left and right subchildren
        removeAllCourse(node->left);
        removeAllCourse(node->right);
        // Afterward delete the node
        delete node;
    }



    /**************************************
     *                                    *
     *          ABCU Application          *
     *                                    *
     **************************************/

    ABCUApplication::ABCUApplication(const std::string& filename): _running(true), _filename(filename) {}

    void ABCUApplication::run()
    {
        while (_running)
        {
            printMenu();
            std::cout << "Enter an option:  ";
            parseUserInput();
        }
    }


    void ABCUApplication::printMenu()
    {
        std::cout << "1. Load dataset\n";
        std::cout << "2. Print all courses (Sorted)\n";
        std::cout << "3. Search course\n";
        std::cout << "9. Exit application\n";
    }

    void ABCUApplication::dispatchAction(const int actionCode)
    {
        std::cout << '\n';

        std::string searchCourseId;
        switch (actionCode)
        {
            case 1:
                _manager.loadCourses(_filename);
                break;
            case 2:
                _manager.printCourses();
                break;
            case 3:
                std::cout << "What are you looking for?  ";
                std::cin >> searchCourseId;
                std::cout << '\n';
                _manager.printCourse(searchCourseId);
                break;
            case 9:
                std::cout << "Exiting application...\n";
                _running = false;
                break;
            default:
                throw std::invalid_argument("Invalid option! Please try again!");
        }
    }


    void ABCUApplication::parseUserInput()
    {

        try
        {
            // Set the iostream to trigger an exception
            std::cin.exceptions(std::ios_base::failbit | std::ios_base::badbit);

            int userInput;
            std::cin >> userInput;
            // Call a subroutine that handles the appropriate action
            dispatchAction(userInput);
        }
        catch (std::exception& e)
        {
            // Print the error message
            std::cout << "Invalid option. Please try again!\n";

            // std::cout << "[Error]: " << e.what() << '\n'; // NOTE: For Diagnosis only.

            // Clear the failbit status
            std::cin.clear();
            // Clear the cin buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // Turn off exceptions
        std::cin.exceptions(std::ios_base::goodbit);
    }

} // namespace abcu



/**************************************
 *                                    *
 *            MAIN FUNCTION           *
 *                                    *
 **************************************/

int main(int argc, char* argv[])
{

    std::string filename = "ABCU_Advising_Program_Input.csv";
    // If a commandline argument is passed-in for filename
    if (argc > 1)
    {
        // Then, that file is used
        filename = argv[1];
    }

    abcu::ABCUApplication app(filename);
    app.run();
}