#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE "credentials.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[50];
char currentRole[20];

void showBanner(const char *title) {
    printf("\n==============================\n");
    printf("  %s\n", title);
    printf("==============================\n");
}

/* Function Declarations */
int login();
void mainMenu();
void adminMenu();
void createRecord();
void showRecords();
void searchRecord();
void editRecord();
void deleteRecord();

int main() {
    showBanner("Welcome to SRMS - By Reshma Somireddy");

    if (login()) {
        printf("\nLogin Successful!\n");
        mainMenu();
    } else {
        printf("\nLogin Failed! Exiting...\n");
    }
    return 0;
}

/******** LOGIN SYSTEM ********/
int login() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20];
    FILE *fp;

    printf("\nEnter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Error: %s file missing!\n", CRE_FILE);
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/******** MAIN MENU ********/
void mainMenu() {
    if (strcmp(currentRole, "Admin") == 0 || strcmp(currentRole, "admin") == 0) {
        adminMenu();
    } else {
        printf("\nOnly Admin access allowed!\n");
    }
}

/******** ADMIN MENU ********/
void adminMenu() {
    int choice;

    while (1) {
        showBanner("ADMIN MENU");
        printf("1. Create Student Record\n");
        printf("2. View All Records\n");
        printf("3. Search Student\n");
        printf("4. Edit Record\n");
        printf("5. Delete Record\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createRecord(); break;
            case 2: showRecords(); break;
            case 3: searchRecord(); break;
            case 4: editRecord(); break;
            case 5: deleteRecord(); break;
            case 6: printf("\nLogging Out...\n"); return;
            default: printf("\nInvalid Choice! Try Again.\n");
        }
    }
}

/******** CREATE RECORD ********/
void createRecord() {
    FILE *fp = fopen(STUD_FILE, "a");
    struct student s;

    if (!fp) {
        printf("Error Opening File!\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &s.roll);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Record Saved Successfully!\n");
}

/******** SHOW RECORDS ********/
void showRecords() {
    FILE *fp = fopen(STUD_FILE, "r");
    struct student s;

    if (!fp) {
        printf("No Records Found!\n");
        return;
    }

    showBanner("STUDENT RECORDS");
    printf("Roll\tName\tMarks\n");

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t%.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}

/******** SEARCH RECORD ********/
void searchRecord() {
    FILE *fp = fopen(STUD_FILE, "r");
    struct student s;
    int roll, found = 0;

    if (!fp) {
        printf("File Not Found!\n");
        return;
    }

    printf("\nEnter Roll To Search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("\nRecord Found!\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No Record Found!\n");

    fclose(fp);
}

/******** EDIT RECORD ********/
void editRecord() {
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct student s;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error Opening File!\n");
        return;
    }

    printf("\nEnter Roll To Edit: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            printf("Enter New Name: ");
            scanf("%s", s.name);
            printf("Enter New Marks: ");
            scanf("%f", &s.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUD_FILE);
        rename("temp.txt", STUD_FILE);
        printf("Record Updated!\n");
    } else {
        remove("temp.txt");
        printf("Record Not Found!\n");
    }
}

/******** DELETE RECORD ********/
void deleteRecord() {
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct student s;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("File Error!\n");
        return;
    }

    printf("\nEnter Roll To Delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(STUD_FILE);
        rename("temp.txt", STUD_FILE);
        printf("Record Deleted!\n");
    } else {
        remove("temp.txt");
        printf("Record Not Found!\n");
    }
}
