#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char firstName[10];
char lastName[10];
char phone[12];  // 11 digits + 1 for null terminator
char address[10];

FILE *file;
FILE *temp;

void addCustomer() {
    system("cls");
    if ((file = fopen("customerInfo.txt", "a")) != NULL) {
        printf("Enter customer details:\n");
        printf("First Name: ");
        scanf("%s", firstName);
        printf("Last Name: ");
        scanf("%s", lastName);

        // Validate phone number length
        do {
            printf("Phone Number (11 digits): ");
            scanf("%s", phone);
            if (strlen(phone) != 11) {
                printf("Phone number must be exactly 11 digits.\n");
            }
        } while (strlen(phone) != 11);

        printf("Address: ");
        scanf("%s", address);

        fprintf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address);
        fclose(file);
        printf("Customer record added successfully.\n");
    } else {
        printf("Failed to open file!\n");
    }
}

void listCustomers() {
    system("cls");
    if ((file = fopen("customerInfo.txt", "r")) != NULL) {
        printf("Registered Customer Details\n");
        printf("---------------------------\n");
        int customerCount = 0;

        while (fscanf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address) == 4) {
            // Check if the phone number is 11 digits
            if (strlen(phone) == 11) {
                customerCount++;
                printf("Customer %d:\n", customerCount);
                printf("First Name  : %s\n", firstName);
                printf("Last Name   : %s\n", lastName);
                printf("Phone Number: %s\n", phone);
                printf("Address     : %s\n", address);
                printf("---------------------------\n");
            } else {
                printf("Data corruption detected for customer: %s %s\n", firstName, lastName);
            }
        }

        if (customerCount == 0) {
            printf("No customers found.\n");
        }
        fclose(file);
    } else {
        printf("Failed to open file!\n");
    }
}


void searchCustomer() {
    system("cls");
    char searchName[10];
    int found = 0;
    if ((file = fopen("customerInfo.txt", "r")) != NULL) {
        printf("Enter the first name of the customer you are searching for: ");
        scanf("%s", searchName);
        printf("Matching Customer Details\n");
        printf("--------------------------\n");
        int customerCount = 0;

        while (fscanf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address) == 4) {
            if (strcmp(firstName, searchName) == 0) {
                customerCount++;
                found = 1;
                printf("Customer %d:\n", customerCount);
                printf("First Name  : %s\n", firstName);
                printf("Last Name   : %s\n", lastName);
                printf("Phone Number: %s\n", phone);
                printf("Address     : %s\n", address);
                printf("--------------------------\n");
            }
        }
        fclose(file);

        if (found == 0) {
            printf("No customer with the first name '%s' found!\n", searchName);
        }
    } else {
        printf("Failed to open file!\n");
    }
}


void deleteCustomer() {
    system("cls");
    char name[20], searchLastName[10];
    int found = 0, sameNameCount = 0;
    if ((file = fopen("customerInfo.txt", "r")) != NULL) {
        printf("Enter the first name of the customer you want to delete: ");
        scanf("%s", name);

        // Check for the number of customers with the same first name
        while (fscanf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address) == 4) {
            if (strcmp(firstName, name) == 0) {
                sameNameCount++;
            }
        }

        rewind(file); // Reset file pointer to the beginning of the file

        if (sameNameCount > 1) {
            printf("Multiple customers with the same first name found. Enter the last name: ");
            scanf("%s", searchLastName);
        } else if (sameNameCount == 1) {
            // Fetch the last name of the single matching record
            while (fscanf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address) == 4) {
                if (strcmp(firstName, name) == 0) {
                    strcpy(searchLastName, lastName);
                    break;
                }
            }
        }

        // Create a temporary file to store remaining customers
        temp = fopen("temp.txt", "w");
        rewind(file); // Reset file pointer again

        while (fscanf(file, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address) == 4) {
            if (strcmp(firstName, name) == 0 && strcmp(lastName, searchLastName) == 0 && found == 0) {
                // Skip the record to be deleted
                found = 1;
            } else {
                // Write the record to the temporary file
                fprintf(temp, "%s\t%s\t%s\t%s\t\n", firstName, lastName, phone, address);
            }
        }

        fclose(file);
        fclose(temp);

        // Replace the original file with the updated file
        remove("customerInfo.txt");
        rename("temp.txt", "customerInfo.txt");

        if (found == 0) {
              printf("Customer named '%s %s' is not in the list!\n", name, searchLastName);
        } else {
            printf("Customer record deleted successfully.\n");
        }

    } else {
        printf("Failed to open file!\n");
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n=== Customer Management System ===\n");
        printf("1- Add New Customer\n");
        printf("2- List All Customers\n");
        printf("3- Search Customer by First Name\n");
        printf("4- Delete Customer\n");
        printf("0- Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input from buffer
            printf("Invalid choice! Please enter a valid option.\n");
            continue;
        }

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                listCustomers();
                break;
            case 3:
                searchCustomer();
                break;
            case 4:
                deleteCustomer();
                break;
            case 0:
                printf("Exiting the system.\n");
                return 0;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    }
}
