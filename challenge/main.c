#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_BOTS 0x20
#define MAX_PORT 65535
#define MAX_URL_SIZE 64
#define MAX_REASON_SIZE 128

typedef struct ZBOT
{
    char *url;
    int port;
} ZBOT;

ZBOT botBuf[MAX_BOTS];

int setup()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    for (size_t i = 0; i < MAX_BOTS; i++)
    {
        botBuf[i].url = NULL;
    }
    return 0;
}

unsigned int getId()
{
    unsigned int id;
    while (1)
    {
        printf("\n[*] Enter bot's id: ");
        scanf("%u", &id);
        getchar();
        if (id >= (unsigned int)MAX_BOTS)
        {
            puts("\n[!] Invalid id.\n");
            continue;
        }
        return id;
    }
}

// i added a new feature i hope you like it
int delete_bot()
{
    unsigned int id;
    id = getId();
    if (botBuf[id].url != NULL)
    {
        char reasonbuf[MAX_REASON_SIZE];
        memset(reasonbuf, '\x00', MAX_REASON_SIZE);
        // gather statistics
        printf("\n[*] Enter the reason of deletion: ");
        read(STDIN_FILENO, &reasonbuf, MAX_REASON_SIZE - 1);
        free(botBuf[id].url);
        botBuf[id].url = NULL;
        puts("\n[+] Bot Deleted successfully! | Reason: ");
        printf(reasonbuf);
        return 0;
    }
    puts("\n[!] Error: Unable to fetch the requested bot entry.\n");
}

char *read_URL()
{
    char *urlBuf;
    urlBuf = malloc(MAX_URL_SIZE);
    char tmp[MAX_URL_SIZE];
    if (urlBuf == NULL)
    {
        printf("\n[!] Error: Memory allocation failed.\n");
    }
    fflush(stdin);
    memset(tmp, '\x00', MAX_URL_SIZE);
    printf("\n[*] Enter ZBot's URL: ");
    if (!fgets(tmp, MAX_URL_SIZE, stdin))
    {
        printf("\n[!] Error: Unable to read the URL.\n");
        exit(1);
    }
    tmp[strcspn(tmp, "\n")] = 0;
    memset(urlBuf, '\x00', MAX_URL_SIZE);
    strncpy(urlBuf, tmp, MAX_URL_SIZE);
    return urlBuf;
}

unsigned int read_port()
{
    unsigned int port;
    while (1)
    {
        printf("\n[*] Enter a listening port (0-65535): ");
        scanf("%u", &port);
        getchar();
        if (port > (unsigned int)MAX_PORT)
        {
            puts("\n[!] The supplied port is not in the (0-65535) range.\n");
            continue;
        }
        return port;
    }
}

int edit_URL(unsigned int id)
{
    char tmp[MAX_URL_SIZE];
    memset(tmp, '\x00', MAX_URL_SIZE);
    printf("\n[*] Enter New URL: ");
    if (!fgets(tmp, MAX_URL_SIZE, stdin))
    {
        printf("\n[!] Error: Unable to read the URL.\n");
        exit(1);
    }
    tmp[strcspn(tmp, "\n")] = 0;
    memset(botBuf[id].url, '\x00', MAX_URL_SIZE);
    strncpy(botBuf[id].url, tmp, MAX_URL_SIZE);
    return 0;
}

int edit_bot()
{
    unsigned int id;
    id = getId();
    if (botBuf[id].url != NULL)
    {
        edit_URL(id);
        botBuf[id].port = read_port();
        printf("\n[+] You updated the bot successfully\n");
        printf("\n[*] New bot settings: URL: %s, Listening port: %u\n", botBuf[id].url, botBuf[id].port);
        return 0;
    }
    puts("\n[!] Error: Unable to fetch the requested bot entry.\n");
    return 0;
}

int add_bot()
{
    for (size_t i = 0; i < MAX_BOTS; i++)
    {
        if (botBuf[i].url == NULL)
        {
            botBuf[i].url = read_URL();
            botBuf[i].port = read_port();
            printf("\n[+] Added a bot with Id: %lu, URL %s and listening port %u\n", i, botBuf[i].url, botBuf[i].port);
            return 0;
        }
    }
    return 0;
}

void flood_attacks(size_t choice, size_t num_of_bots){
  char attack[200] = {0};
  char temp_num[20] = {0};
  strcpy(attack, "\n[*] Performing ");
  
  if (choice == 4)
   strcat(attack, "HTTP flood attack with ");
  else if (choice == 5)
   strcat(attack, "TCP stomp flood attack with ");
  else if (choice == 6)
   strcat(attack, "Optimized UDP flood attack with ");
  else if (choice == 5)
   strcat(attack, "DNS Resolver flood attack with ");

  sprintf(temp_num, "%ld ", num_of_bots);
  strcat(attack, temp_num); 
  strcat(attack, "bots...\n");
  for (size_t i = 0; i < strlen(attack); i++){
    putchar(attack[i]);
    if (attack[i] == '.')
      sleep(1);
    usleep(10000);
  }
  sleep(0.5);
}

int menu()
{
    char input[3];
    memset(&input, 0, 3);

    srand(time(NULL));
    size_t num_of_bots = rand() % 1000000;
    while (1)
    {
        printf("\n[*] Current number of bots available: [%ld] \n\n", num_of_bots);
        printf("[1] - Add a bot\n");
        printf("[2] - Edit a bot\n");
        printf("[3] - Delete a bot\n");
        printf("[4] - HTTP Flood\n");
        printf("[5] - TCP stomp Flood\n");
        printf("[6] - Optimized UDP Flood\n");
        printf("[7] - DNS Resolver Flood\n");
        printf("\n>> ");
        read(0, &input, 2);
        switch (input[0])
        {
        case '1':
            add_bot();
            break;
        case '2':
            edit_bot();
            break;
        case '3':
            delete_bot();
            break;
        case '4':
            flood_attacks(4, num_of_bots);
            break;
        case '5':
            flood_attacks(5, num_of_bots);
            break;
        case '6':
            flood_attacks(6, num_of_bots);
            break;
        case '7': 
            flood_attacks(7, num_of_bots);
            break;
        default:
            printf("\n[-] Invalid option!\n");
            continue;
        }
    }
}

int main(int argc, char *argv[])
{
    setup();
    puts("\nZoldyck Botnet");
    menu();
    return 0;
}
