#include "addressbook.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* =========================================================
                       ADD CONTACT
   ========================================================*/

void load_contacts(struct AddressBook *addressBook)
{
    FILE *fp;

    fp = fopen("database.csv", "r");

    if (fp == NULL)
    {
        printf("Database file not found.\n");
        printf("Starting with empty address book.\n");
        return;
    }

    addressBook->contactCount = 0;

    while (addressBook->contactCount < MAX_CONTACTS)
    {
        if (fscanf(fp, "%49[^,],%19[^,],%49[^\n]\n",
                   addressBook->contacts[addressBook->contactCount].name,
                   addressBook->contacts[addressBook->contactCount].phone,
                   addressBook->contacts[addressBook->contactCount].email) != 3)
        {
            break;
        }

        addressBook->contactCount++;
    }

    fclose(fp);

    printf("%d contacts loaded successfully.\n",
           addressBook->contactCount);
}



void add_contact(struct AddressBook *addressBook)
{
    int valid;
    int duplicate;

    if (addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("Address book is full.\n");
        return;
    }

    /* =====================================================
                           NAME
       ===================================================== */

    do
    {
        valid = 1;

        printf("Enter the name: ");

        scanf(" %49[^\n]",
              addressBook->contacts[addressBook->contactCount].name);

        char *name =
            addressBook->contacts[addressBook->contactCount].name;

        /* Check empty name */
        if (strlen(name) == 0)
        {
            valid = 0;
            printf("Name cannot be empty.\n");
        }

    } while (valid == 0);


    /* =====================================================
                       PHONE NUMBER
       ===================================================== */

    do
    {
        valid = 1;
        duplicate = 0;

        printf("Enter the mobile number: ");

        scanf("%19s",
              addressBook->contacts[addressBook->contactCount].phone);

        char *phone =
            addressBook->contacts[addressBook->contactCount].phone;

        /* Check length */
        if (strlen(phone) != 10)
        {
            valid = 0;
        }

        /* Check digits */
        if (valid == 1)
        {
            for (int i = 0; i < 10; i++)
            {
                if (!isdigit((unsigned char)phone[i]))
                {
                    valid = 0;
                    break;
                }
            }
        }

        /* First digit must be 6-9 */
        if (valid == 1)
        {
            if (phone[0] < '6' || phone[0] > '9')
            {
                valid = 0;
            }
        }

        /* Check duplicate phone */
        if (valid == 1)
        {
            for (int i = 0;
                 i < addressBook->contactCount;
                 i++)
            {
                if (strcmp(phone,
                           addressBook->contacts[i].phone) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }
        }

        if (valid == 0)
        {
            printf("Invalid mobile number.\n");
            printf("Phone must contain exactly 10 digits\n");
            printf("and first digit must be 6, 7, 8 or 9.\n");
        }
        else if (duplicate == 1)
        {
            printf("Phone number already exists.\n");
        }

    } while (valid == 0 || duplicate == 1);


    /* =====================================================
                          EMAIL
       ===================================================== */

    do
    {
        valid = 1;
        duplicate = 0;

        printf("Enter the email: ");

        scanf("%49s",
              addressBook->contacts[addressBook->contactCount].email);

        char *email =
            addressBook->contacts[addressBook->contactCount].email;

        int len = strlen(email);
        int at_count = 0;
        int at_pos = -1;

        /* Find @ */
        for (int i = 0; i < len; i++)
        {
            if (email[i] == '@')
            {
                at_count++;
                at_pos = i;
            }
        }

        /* Exactly one @ */
        if (at_count != 1)
        {
            valid = 0;
        }

        /* @ cannot be first */
        if (at_pos <= 0)
        {
            valid = 0;
        }

        /* @ cannot be last */
        if (at_pos == len - 1)
        {
            valid = 0;
        }

        /* Check .com */
        if (len < 5 ||
            email[len - 4] != '.' ||
            email[len - 3] != 'c' ||
            email[len - 2] != 'o' ||
            email[len - 1] != 'm')
        {
            valid = 0;
        }

        /* Convert to lowercase */
        for (int i = 0; email[i] != '\0'; i++)
        {
            email[i] = tolower((unsigned char)email[i]);
        }

        /* Check duplicate email */
        if (valid == 1)
        {
            for (int i = 0;
                 i < addressBook->contactCount;
                 i++)
            {
                if (strcmp(email,
                           addressBook->contacts[i].email) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }
        }

        if (valid == 0)
        {
            printf("Invalid email.\n");
            printf("Example: example@gmail.com\n");
        }
        else if (duplicate == 1)
        {
            printf("Email already exists.\n");
        }

    } while (valid == 0 || duplicate == 1);


    /* =====================================================
                       INCREASE COUNT
       ===================================================== */

    addressBook->contactCount++;

    /* Save database */
    save_contacts(addressBook);

    printf("Contact added successfully!\n");
}
/*Search contact*/
void search_contact(struct AddressBook *addressBook)
{
    char search[50];
    int choice;
    int found = 0;

    if (addressBook->contactCount == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    printf("\n");
    printf("Search contact by:\n");
    printf("1. Name\n");
    printf("2. Phone\n");
    printf("3. Email\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 3)
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("Enter the value to search: ");

    
    scanf(" %49[^\n]", search);

    printf("\n");

    printf("+------+-------------------------+--------------+------------------------------+\n");
    printf("| S.No | Name                    | Phone        | Email                        |\n");
    printf("+------+-------------------------+--------------+------------------------------+\n");

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        int match = 0;

        if (choice == 1)
        {
            if (strcmp(search, addressBook->contacts[i].name) == 0)
            {
                match = 1;
            }
        }
        else if (choice == 2)
        {
            if (strcmp(search, addressBook->contacts[i].phone) == 0)
            {
                match = 1;
            }
        }
        else if (choice == 3)
        {
            if (strcmp(search, addressBook->contacts[i].email) == 0)
            {
                match = 1;
            }
        }

        if (match == 1)
        {
            printf("| %-4d | %-23s | %-12s | %-28s |\n",
                   i + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            found = 1;
        }
    }

    printf("+------+-------------------------+--------------+------------------------------+\n");

    if (found == 0)
    {
        printf("Contact not found.\n");
    }
}


void edit_contact(struct AddressBook *addressBook)
{
    char search[50];

    int index_record[MAX_CONTACTS];
    int count = 0;

    int search_choice;
    int choice;
    int index;

    /* =====================================================
                       CHECK CONTACTS
       ===================================================== */

    if (addressBook->contactCount == 0)
    {
        printf("No contacts found.\n");
        return;
    }

    /* =====================================================
                         SEARCH MENU
       ===================================================== */

    printf("\n");
    printf("Search contact by:\n");
    printf("1. Search by name\n");
    printf("2. Search by phone\n");
    printf("3. Search by email\n");

    printf("Enter your choice: ");
    scanf("%d", &search_choice);

    if (search_choice < 1 || search_choice > 3)
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("Enter the value to search: ");

    scanf(" %49[^\n]", search);

    /* =====================================================
                       FIND CONTACTS
       ===================================================== */

    for (int i = 0;
         i < addressBook->contactCount;
         i++)
    {
        int match = 0;

        if (search_choice == 1)
        {
            if (strcmp(search,
                       addressBook->contacts[i].name) == 0)
            {
                match = 1;
            }
        }

        else if (search_choice == 2)
        {
            if (strcmp(search,
                       addressBook->contacts[i].phone) == 0)
            {
                match = 1;
            }
        }

        else if (search_choice == 3)
        {
            if (strcmp(search,
                       addressBook->contacts[i].email) == 0)
            {
                match = 1;
            }
        }

        if (match == 1)
        {
            index_record[count] = i;
            count++;
        }
    }

    /* =====================================================
                         NOT FOUND
       ===================================================== */

    if (count == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    /* =====================================================
                   DISPLAY MATCHING CONTACTS
       ===================================================== */

    printf("\nMatching contacts:\n");

    for (int i = 0; i < count; i++)
    {
        index = index_record[i];

        printf("%d. %s | %s | %s\n",
               i + 1,
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
    }

    /* =====================================================
                       SELECT CONTACT
       ===================================================== */

    do
    {
        printf("\nPlease select your contact: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > count)
        {
            printf("Invalid choice. Try again.\n");
        }

    } while (choice < 1 || choice > count);

    index = index_record[choice - 1];

    /* =====================================================
                         EDIT MENU
       ===================================================== */

    do
    {
        printf("\n");
        printf("What do you want to edit?\n");
        printf("1. Name\n");
        printf("2. Phone\n");
        printf("3. Email\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        /* =================================================
                             NAME
           ================================================= */

        if (choice == 1)
        {
            char new_name[NAME_LEN];
            int duplicate = 0;

            printf("Enter new name: ");

            scanf(" %49[^\n]", new_name);

            if (strlen(new_name) == 0)
            {
                printf("Name cannot be empty.\n");
                continue;
            }

            /* Check duplicate name */

            for (int i = 0;
                 i < addressBook->contactCount;
                 i++)
            {
                if (i != index &&
                    strcmp(new_name,
                           addressBook->contacts[i].name) == 0)
                {
                    duplicate = 1;
                    break;
                }
            }

            if (duplicate == 1)
            {
                printf("Name already exists.\n");
            }
            else
            {
                strcpy(addressBook->contacts[index].name,
                       new_name);

                save_contacts(addressBook);

                printf("Name updated successfully.\n");
            }
        }

        /* =================================================
                            PHONE
           ================================================= */

        else if (choice == 2)
        {
            char new_phone[PHONE_LEN];

            int valid;
            int duplicate;

            do
            {
                valid = 1;
                duplicate = 0;

                printf("Enter new phone: ");

                scanf("%19s", new_phone);

                /* Check length */

                if (strlen(new_phone) != 10)
                {
                    valid = 0;
                }

                /* Check digits */

                if (valid == 1)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        if (!isdigit(
                                (unsigned char)new_phone[i]))
                        {
                            valid = 0;
                            break;
                        }
                    }
                }

                /* First digit */

                if (valid == 1)
                {
                    if (new_phone[0] < '6' ||
                        new_phone[0] > '9')
                    {
                        valid = 0;
                    }
                }

                /* Check duplicate */

                if (valid == 1)
                {
                    for (int i = 0;
                         i < addressBook->contactCount;
                         i++)
                    {
                        if (i != index &&
                            strcmp(new_phone,
                                   addressBook->contacts[i].phone) == 0)
                        {
                            duplicate = 1;
                            break;
                        }
                    }
                }

                if (valid == 0)
                {
                    printf("Invalid phone number.\n");
                    printf("Phone must contain exactly 10 digits\n");
                    printf("and first digit must be 6, 7, 8 or 9.\n");
                }
                else if (duplicate == 1)
                {
                    printf("Phone number already exists.\n");
                }

            } while (valid == 0 || duplicate == 1);

            strcpy(addressBook->contacts[index].phone,
                   new_phone);

            save_contacts(addressBook);

            printf("Phone updated successfully.\n");
        }

        /* =================================================
                            EMAIL
           ================================================= */

        else if (choice == 3)
        {
            char new_email[EMAIL_LEN];

            int valid;
            int duplicate;

            do
            {
                valid = 1;
                duplicate = 0;

                printf("Enter new email: ");

                scanf("%49s", new_email);

                int len = strlen(new_email);
                int at_count = 0;
                int at_pos = -1;

                /* Find @ */

                for (int i = 0; i < len; i++)
                {
                    if (new_email[i] == '@')
                    {
                        at_count++;
                        at_pos = i;
                    }
                }

                /* Exactly one @ */

                if (at_count != 1)
                {
                    valid = 0;
                }

                /* @ not first */

                if (at_pos <= 0)
                {
                    valid = 0;
                }

                /* @ not last */

                if (at_pos == len - 1)
                {
                    valid = 0;
                }

                /* Check .com */

                if (len < 5 ||
                    new_email[len - 4] != '.' ||
                    new_email[len - 3] != 'c' ||
                    new_email[len - 2] != 'o' ||
                    new_email[len - 1] != 'm')
                {
                    valid = 0;
                }

                /* Convert to lowercase */

                for (int i = 0;
                     new_email[i] != '\0';
                     i++)
                {
                    new_email[i] =
                        tolower((unsigned char)new_email[i]);
                }

                /* Duplicate email */

                if (valid == 1)
                {
                    for (int i = 0;
                         i < addressBook->contactCount;
                         i++)
                    {
                        if (i != index &&
                            strcmp(new_email,
                                   addressBook->contacts[i].email) == 0)
                        {
                            duplicate = 1;
                            break;
                        }
                    }
                }

                if (valid == 0)
                {
                    printf("Invalid email.\n");
                    printf("Example: example@gmail.com\n");
                }
                else if (duplicate == 1)
                {
                    printf("Email already exists.\n");
                }

            } while (valid == 0 || duplicate == 1);

            strcpy(addressBook->contacts[index].email,
                   new_email);

            save_contacts(addressBook);

            printf("Email updated successfully.\n");
        }

        /* =================================================
                            EXIT
           ================================================= */

        else if (choice == 4)
        {
            printf("Exiting edit menu...\n");
        }

        else
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 4);
}

void delete_contact(struct AddressBook *addressBook)
{
    char search[50];
    char confirm;

    int index_record[MAX_CONTACTS];
    int count = 0;

    int search_choice;
    int choice;
    int index;

    /* =====================================================
                     CHECK CONTACTS
       ===================================================== */

    if (addressBook->contactCount == 0)
    {
        printf("No contacts found.\n");
        return;
    }

    /* =====================================================
                       SEARCH MENU
       ===================================================== */

    printf("\n");
    printf("Search contact by:\n");
    printf("1. Search by name\n");
    printf("2. Search by phone\n");
    printf("3. Search by email\n");

    printf("Enter your choice: ");
    scanf("%d", &search_choice);
    getchar();

    if (search_choice < 1 || search_choice > 3)
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("Enter the value to search: ");

    fgets(search, sizeof(search), stdin);

    search[strcspn(search, "\r\n")] = '\0';

    /* =====================================================
                       FIND CONTACTS
       ===================================================== */

    for (int i = 0;
         i < addressBook->contactCount;
         i++)
    {
        int match = 0;

        if (search_choice == 1)
        {
            if (strcmp(search,
                       addressBook->contacts[i].name) == 0)
            {
                match = 1;
            }
        }

        else if (search_choice == 2)
        {
            if (strcmp(search,
                       addressBook->contacts[i].phone) == 0)
            {
                match = 1;
            }
        }

        else if (search_choice == 3)
        {
            if (strcmp(search,
                       addressBook->contacts[i].email) == 0)
            {
                match = 1;
            }
        }

        if (match == 1)
        {
            index_record[count] = i;
            count++;
        }
    }

    /* =====================================================
                         NOT FOUND
       ===================================================== */

    if (count == 0)
    {
        printf("Contact not found.\n");
        return;
    }

    /* =====================================================
                 DISPLAY MATCHING CONTACTS
       ===================================================== */

    printf("\nMatching contacts:\n");

    for (int i = 0; i < count; i++)
    {
        index = index_record[i];

        printf("%d. %s | %s | %s\n",
               i + 1,
               addressBook->contacts[index].name,
               addressBook->contacts[index].phone,
               addressBook->contacts[index].email);
    }

    /* =====================================================
                       SELECT CONTACT
       ===================================================== */

    do
    {
        printf("\nPlease select the contact to delete: ");

        scanf("%d", &choice);
        getchar();

        if (choice < 1 || choice > count)
        {
            printf("Invalid choice. Try again.\n");
        }

    } while (choice < 1 || choice > count);

    index = index_record[choice - 1];

    /* =====================================================
                    DISPLAY SELECTED
       ===================================================== */

    printf("\nSelected contact:\n");

    printf("Name  : %s\n",
           addressBook->contacts[index].name);

    printf("Phone : %s\n",
           addressBook->contacts[index].phone);

    printf("Email : %s\n",
           addressBook->contacts[index].email);

    /* =====================================================
                         CONFIRM
       ===================================================== */

    do
    {
        printf("\nDo you want to delete? (y/n): ");

        scanf(" %c", &confirm);
        getchar();

        if (confirm != 'y' &&
            confirm != 'Y' &&
            confirm != 'n' &&
            confirm != 'N')
        {
            printf("Invalid choice. Try again.\n");
        }

    } while (confirm != 'y' &&
             confirm != 'Y' &&
             confirm != 'n' &&
             confirm != 'N');

    /* =====================================================
                         CANCEL
       ===================================================== */

    if (confirm == 'n' || confirm == 'N')
    {
        printf("Deletion cancelled.\n");
        return;
    }

    /* =====================================================
                     SHIFT CONTACTS
       ===================================================== */

    for (int i = index;
         i < addressBook->contactCount - 1;
         i++)
    {
        addressBook->contacts[i] =
            addressBook->contacts[i + 1];
    }

    /* =====================================================
                     DECREASE COUNT
       ===================================================== */

    addressBook->contactCount--;

    /* =====================================================
                     SAVE DATABASE
       ===================================================== */

    save_contacts(addressBook);

    printf("Contact deleted successfully.\n");
}


void save_contacts(struct AddressBook *addressBook)
{
    FILE *fp;

    fp = fopen("database.csv", "w");

    if (fp == NULL)
    {
        printf("Error opening database.csv\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fp,
                "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);

    printf("Contacts saved successfully.\n");
}


void list_contacts(struct AddressBook *addressBook)
{
    const char *RESET = "\033[0m";
    const char *NAME_COLOR = "\033[1;32m";
    const char *PHONE_COLOR = "\033[1;33m";
    const char *EMAIL_COLOR = "\033[1;36m";

    if (addressBook->contactCount == 0)
    {
        printf("\nNo contacts found.\n");
        return;
    }

    printf("\n");

    printf("+-----+-------------------------+------------------+--------------------------------+\n");
    printf("| No. | Name                    | Phone            | Email                          |\n");
    printf("+-----+-------------------------+------------------+--------------------------------+\n");

    for (int i = 0;
         i < addressBook->contactCount;
         i++)
    {
        printf("| %-3d | ", i + 1);

        printf("%s%-23s%s | ",
               NAME_COLOR,
               addressBook->contacts[i].name,
               RESET);

        printf("%s%-16s%s | ",
               PHONE_COLOR,
               addressBook->contacts[i].phone,
               RESET);

        printf("%s%-30s%s |\n",
               EMAIL_COLOR,
               addressBook->contacts[i].email,
               RESET);
    }

    printf("+-----+-------------------------+------------------+--------------------------------+\n");

    printf("Total contacts: %d\n",
           addressBook->contactCount);
}