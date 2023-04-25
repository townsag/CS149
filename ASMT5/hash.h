#ifndef HASH_H
#define HASH_H

#define HASH_SIZE 101

struct nlist {
        /* table entry: */
        struct nlist *next; /* next entry in chain */
        char* command;
        char *defn; /* replacement text, can remove */
        /* starttime */
        /* finishtime */
        /* index // this is the line index in the input text file */
        /* pid // the process id. you can use the pid result of wait to lookup in the hashtable */
        /* char *command; // command. This is good to store for when you decide to restart a command */
};
