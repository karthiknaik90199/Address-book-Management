#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CONTACTS 100
#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50

struct Contact
{
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
};

struct AddressBook
{
    struct Contact contacts[MAX_CONTACTS];
    int contactCount;
};


void load_contacts(struct AddressBook *addressBook);
void save_contacts(struct AddressBook *addressBook);

void add_contact(struct AddressBook *addressBook);
void search_contact(struct AddressBook *addressBook);
void edit_contact(struct AddressBook *addressBook);
void delete_contact(struct AddressBook *addressBook);
void list_contacts(struct AddressBook *addressBook);

#endif