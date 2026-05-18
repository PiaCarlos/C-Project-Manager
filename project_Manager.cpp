#include <string>
#include <iostream>
using namespace std;

class TeamMember {
private:
    int memberId;
    string name;
    string role;
    int maxTasks; 

public:
    TeamMember(int memberId, string name, string role, int maxTasks) 
    : memberId(memberId), name(name), role(role), maxTasks(maxTasks) {} // more efficient way of doing a constructor

    ~TeamMember() = default; // no dynamic memory alocation, so default is enough

    TeamMember(const TeamMember& other) = default;

    void display() const{
        cout << "ID: " << memberId << ", Name: " 
        << name << ", Role: " << role
        << ", Max Tasks: " << maxTasks << '\n';
    }

    // methods are const, since they are getters
    int getMemberId() const{
        return memberId;
    }

    string getName() const{
        return name;
    }

    int getMaxTasks() const{
        return maxTasks;
    }

};

class Task {
private:
    int taskId;
    string description;
    int priority;
    string status;
public:
    // adding const for the strings, make sure they are not modified when constructing. Not need to make a copy.
    Task(int taskId, const string& description, int priority, const string& status) 
        : taskId(taskId), description(description), priority(priority), status(status) {} // more efficient way of doing a constructor

    ~Task() = default; 

    Task(const Task& other) = default;

    void updateStatus(const string& newStatus){
        status = newStatus;
    }

    void updatePriority(int newPriority){
        priority = newPriority;
    }

    void display(const string& assignedMemberName = "None") const{
        cout << "Task ID: " << taskId 
        << ", Description: " << description 
        << ", Priority:" << priority 
        << ", Status: " << status 
        << ", Assigned to: " << assignedMemberName << '\n';
    }

    int getTaskId() const{
        return taskId;
    }

    int getPriority() const{
        return priority;
    }

    string getStatus() const{
        return status;
    }

  
};


class ProjectManager {
private:
    int* projectIds;
    string* projectNames;
    int numProjects;
    int maxProjects;
    Task** tasks; // ** since array of pointers
    int* taskProjectIds;
    TeamMember** taskAssignments;
    int numTasks;
    int maxTasks;
    TeamMember** teamMembers;
    int numMembers;
    int maxMembers;

public:
    ProjectManager(int maxProjects, int maxTasks, int maxMembers) 
    : maxProjects(maxProjects), maxTasks(maxTasks), maxMembers(maxMembers), 
      numProjects(0), numTasks(0), numMembers(0){
        // the counters set to  0 since we have no projects, tasks or team members
        // set arrays
        projectIds       = new int[maxProjects];
        projectNames     = new string[maxProjects];
        tasks            = new Task*[maxTasks];
        taskProjectIds   = new int[maxTasks];
        taskAssignments  = new TeamMember*[maxTasks];
        teamMembers      = new TeamMember*[maxMembers];

        // initialize task and team member pointers to nullptr
        for(int i = 0; i < maxTasks; i++){
            tasks[i] = nullptr;
            taskAssignments[i] = nullptr;
        }
        for(int i = 0; i < maxMembers; i++){
            teamMembers[i] = nullptr;
        }
    }
        
    ~ProjectManager() {
        delete[] projectIds;
        delete[] projectNames;
        delete[] taskProjectIds;

        // to delete array of pointers, delete pointers inside, then delete array
        for(int i = 0; i < numTasks; i++){
            if(tasks[i] != nullptr){
                delete tasks[i];
            }
        }

        for(int i = 0; i < numMembers; i++){
            if(teamMembers[i] != nullptr){
                delete teamMembers[i];
            }
        }

        // no need to delete taskAssignments since they point to teamMembers, which are already deleted.

        delete[] tasks;
        delete[] taskAssignments;
        delete[] teamMembers;
    }   

    // make copy constructor
    ProjectManager(const ProjectManager& other) {
        numProjects = other.numProjects;
        maxProjects = other.maxProjects;
        numTasks    = other.numTasks;
        maxTasks    = other.maxTasks;
        numMembers  = other.numMembers;
        maxMembers  = other.maxMembers;

        // for the arrays, need to create arrays then copy content. 
        projectIds      = new int[maxProjects];
        projectNames    = new string[maxProjects];
        taskProjectIds  = new int[maxTasks];
        tasks           = new Task*[maxTasks];
        taskAssignments = new TeamMember*[maxTasks];
        teamMembers     = new TeamMember*[maxMembers];

        // initialize task and team member pointers to nullptr
        for(int i = numTasks; i < maxTasks; i++){
            tasks[i] = nullptr;
            taskAssignments[i] = nullptr;
        }

        for(int i = numMembers; i < maxMembers; i++){
            teamMembers[i] = nullptr;
        }


        // copy contents into them.
        for(int i = 0; i < numProjects; i++){
            projectIds[i] = other.projectIds[i];
            projectNames[i] = other.projectNames[i];
        }
        for(int i= 0; i < numMembers; i++) {
            teamMembers[i] = new TeamMember(*other.teamMembers[i]);
        }
        for(int i = 0; i < numTasks; i++){
            tasks[i] = new Task(*other.tasks[i]);
            taskProjectIds[i] = other.taskProjectIds[i];

            // get index from team members, since they to point to the new team members, not to the old team members.
            taskAssignments[i] = nullptr;
            for(int j = 0; j < numMembers; j++){
                if(other.taskAssignments[i] == other.teamMembers[j]){
                    taskAssignments[i] = teamMembers[j];
                    break;
                }
            }
        }

        
    }


    void createProject(int projectId, const string& projectName) {
        if(numProjects >= maxProjects){
            cout << "max projects reached. \n";
            return;
        }

        projectIds[numProjects]   = projectId;
        projectNames[numProjects] = projectName;
        numProjects++;
    }

    void addTeamMember(int memberId, const string& name, const string& role, int maxTasks) {
        if(numMembers >= maxMembers){
            cout << "max team members reached. \n";
            return;
        }

        teamMembers[numMembers] = new TeamMember(memberId, name, role, maxTasks);
        numMembers++;
    }

    void createTask(int projectId, int taskId, const string& description, int priority, const string& status){
        if(numTasks >= maxTasks){
            cout << "max tasks reached. \n";
            return;
        }

        tasks[numTasks] = new Task(taskId, description, priority, status);
        taskProjectIds[numTasks] = projectId;
        taskAssignments[numTasks] = nullptr; // no member assigned yet
        numTasks++;
    }

    void assignTaskToMember(int projectId, int taskId, int memberId)  {
        // find task
        int i = 0;
        int taskIndex = -1;
        while(i < numTasks){
            int currentProjectId = taskProjectIds[i];
            int currentTaskId = tasks[i]->getTaskId();
            if(currentProjectId == projectId && currentTaskId == taskId){
                taskIndex = i;
                break;
            }
            i++;
        }

        // if no found, error
        if(taskIndex == -1){
            cout << "task not found. \n";
            return;
        }

        // find member
        int j = 0;
        int memberIndex = -1;
        while(j < numMembers){
            int currentMemberId= teamMembers[j]->getMemberId();
            if(currentMemberId == memberId){
                memberIndex = j;
                break;
            }
            j++;
        }

        // if no found, error
        if(memberIndex == -1){
            cout << "team member not found. \n";
            return;
        }

        // check if member can take more tasks
        int counterTasks = 0;
        int k = 0;
        while(k < numTasks){
            if(taskAssignments[k] == teamMembers[memberIndex]){
                counterTasks++;
            }
            k++;
        }

        if(counterTasks >= teamMembers[memberIndex]->getMaxTasks()){ 
            cout << "team member has max number of tasks. \n";
            return;
        }

        // assign task to member
        taskAssignments[taskIndex] = teamMembers[memberIndex];
    }

    TeamMember* getAssignedMember(int projectId, int taskId) {
        // find task
        int i = 0;
        int taskIndex = -1;
        while(i < numTasks){
            int currentProjectId = taskProjectIds[i];
            int currentTaskId = tasks[i]->getTaskId();
            if(currentProjectId == projectId && currentTaskId == taskId){
                taskIndex = i;
                break;
            }
            i++;
        }

        // if no found, error
        if(taskIndex == -1){
            cout << "task not found. \n";
            return nullptr;
        }

        TeamMember* assignedMember = taskAssignments[taskIndex];

        if(assignedMember == nullptr){
            cout << "no member assigned to the task. \n";
            return nullptr;
        }

        return assignedMember;
        
    }

    Task** getTasksForMember(int memberId, int& count) {
        // find member
        int j = 0;
        int memberIndex = -1;
        while(j < numMembers){
            int currentMemberId= teamMembers[j]->getMemberId();
            if(currentMemberId == memberId){
                memberIndex = j;
                break;
            }
            j++;
        }

        // if no found, error
        if(memberIndex == -1){
            cout << "team member not found. \n";
            return nullptr;
        }

        // get tasks for member
        Task** memberTasks = new Task*[numTasks]; 
        int countTasks = 0;
        int i = 0;

        while(i < numTasks){
            if(taskAssignments[i] == teamMembers[memberIndex]) {
                memberTasks[countTasks] = tasks[i];
                countTasks++;
            }
            i++;
        }
       
        count = countTasks; // update count since &count is a reference

        return memberTasks;
    }


    Task** getTasksByStatus(int projectId, const string& status, int& count) {
        // get tasks for project that has some status
        Task** statusTasks = new Task*[numTasks];
        int countStatusTasks = 0;
        int i = 0;

        while(i < numTasks){
            int currentProjectId = taskProjectIds[i];
            string currentStatus = tasks[i]->getStatus();
            if(currentProjectId == projectId && currentStatus == status) {
                statusTasks[countStatusTasks] = tasks[i];
                countStatusTasks++;
            }
            i++;    
        }

        count = countStatusTasks; // update count since &count is a reference

        return statusTasks;
    }

    bool findProject(int projectId) {
        int i = 0;
        bool foundProject = false;
        while(i < numProjects){
            if(projectIds[i] == projectId){
                foundProject = true;  // found project
            }
            i++;
        }

        // project does not exist
        return foundProject;
    }

    TeamMember* findMember(int memberId) {
        int i = 0;
        while(i < numMembers){
            int currentTeamMemberId = teamMembers[i]->getMemberId();
            if(currentTeamMemberId == memberId){
                return teamMembers[i]; // found member
            }
            i++;
        }

        return nullptr;
    }

    void displayAllProjects() {
        int i = 0;
        // display projects
        while(i < numProjects){
            cout << "Project ID: " << projectIds[i] << ",Name: " << projectNames[i] << "\n";
            // display their tasks
            int j = 0;
            while(j < numTasks){
                if(taskProjectIds[j] == projectIds[i]){
                    string memberName = "None";
                    if(taskAssignments[j] != nullptr){
                        memberName = taskAssignments[j]->getName();
                    }
                    tasks[j]->display(memberName); // call display method we already made on Task
                }
                j++;
            }

            i++;
        }
    }

    void displayTeamMembers() {
        int i = 0;
        // display members 
        while(i < numMembers){
            teamMembers[i]->display(); // call display method we already made on TeamMember
            // display their tasks
            int j = 0;
            while(j < numTasks){
                if(taskAssignments[j] == teamMembers[i]){
                    string memberName = "None";
                    memberName = taskAssignments[j]->getName();
                    tasks[j]->display(memberName); // call display method we already made on Task
                }
                j++;
            }
            i++;
        }
    }

    void displayTasksByStatus(int projectId, const string& status) {
        int i = 0;

        while(i < numTasks){
            int currentProjectId = taskProjectIds[i];
            string currentStatus = tasks[i]->getStatus();
            if(currentProjectId == projectId && currentStatus == status){

                string memberName = "None";
                if(taskAssignments[i] != nullptr){
                    memberName = taskAssignments[i]->getName();
                }

                tasks[i]->display(memberName); // call display method we already made on Task
            }
            i++;    
        }
    }

};


int main() {
    ProjectManager manager(5, 50, 10); // Max 5 projects, 50 tasks, 10 team members
    // Add team members
    manager.addTeamMember(101, "Alice Chen", "Developer", 5);
    manager.addTeamMember(102, "Bob Wilson", "Designer", 3);
    manager.addTeamMember(103, "Carol Davis", "Manager", 10);
    // Create projects
    manager.createProject(1, "Website Redesign");
    manager.createProject(2, "Mobile App");
    // Create tasks
    manager.createTask(1, 1001, "Design homepage layout", 2, "To Do");
    manager.createTask(1, 1002, "Implement navigation menu", 1, "In Progress");
    manager.createTask(1, 1003, "Write unit tests", 3, "To Do");
    manager.createTask(2, 2001, "Create app wireframes", 1, "Completed");
    manager.createTask(2, 2002, "Set up development environment", 2, "In Progress");
    // Assign tasks to team members
    manager.assignTaskToMember(1, 1001, 102); // Assign design task to Bob (Designer)
    manager.assignTaskToMember(1, 1002, 101); // Assign implementation to Alice (Developer)
    manager.assignTaskToMember(2, 2001, 102); // Assign wireframes to Bob
    manager.assignTaskToMember(2, 2002, 101); // Assign setup to Alice
    // Display system state
    std::cout << "=== All Projects ===" << std::endl;
    manager.displayAllProjects();
    std::cout << "\n=== Team Members ===" << std::endl;
    manager.displayTeamMembers();
    std::cout << "\n=== Tasks in Progress (Project 1) ===" << std::endl;
    manager.displayTasksByStatus(1, "In Progress");

    // extra tests 
    std::cout << "\n=== Extra tests ===" << std::endl;

    // test getAssignedMember
    TeamMember* testAssignedMember = manager.getAssignedMember(1, 1001);
    cout << "Assigned member for task 1001 to" << testAssignedMember->getName() << "\n";

    // test getTasksForMember
    int counterAlice = 0;
    Task** aliceTasks = manager.getTasksForMember(101, counterAlice);
    cout << "Alice tasks: \n";
    int i = 0;
    while(i < counterAlice){
        aliceTasks[i]->display();
        i++;
    }

    // have to delete aliceTasks since used new
    delete[] aliceTasks;

    // test find project and member
    bool foundProject = manager.findProject(1);
    if(foundProject){
        cout << "Project 1 was found. " << "\n";
    } else {
        cout << "Project 1 is not found" << "\n";
    }

    TeamMember* foundMember = manager.findMember(102);
    if(foundMember != nullptr){
        cout << "Member 102 was found:" << foundMember->getName() << "\n";
    } else {
        cout << "Member 102 was not found. " << "\n";
    }

    // test getTasksByStatus
    int counter = 0;
    Task** inProgressTasks = manager.getTasksByStatus(1, "To Do", counter);
    cout << "To Do tasks used in project 1: " << counter << "\n";
    i = 0;
    while(i < counter){
        inProgressTasks[i]->display();
        i++;
    }
    // have to delete inProgressTasks since used new
    delete[] inProgressTasks;


    // test copy constructor
    ProjectManager managerCopy = manager; 
    std::cout << "\n=== Copy ===" << std::endl;
    managerCopy.displayAllProjects();

    return 0;
}