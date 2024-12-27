#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Global Value
#define MAX_NAME 100
#define MAX_OP 20

//Functions
void getinput(char line[], char op[]);
void wData(char data[]);
void lData(void);
void rmData(char line[]);


int main()
{
    char *name = malloc(sizeof(char) * MAX_NAME);
    char *op = malloc(sizeof(char) * MAX_OP);
     
     if (name == NULL)
     {
        printf("NO Memory");
        return 1;
     }

        while (1) {
        getinput(name, op);

        if (strcmp(op, "exit") == 0) 
        {
            printf("Exiting program...\n");
            free(name);
            free(op);
            return 0;
        }

        // Switch according to operation command
        if (strcmp(op, "add") == 0 || strcmp(op, "ADD") == 0 || strcmp(op, "Add") == 0 || strcmp(op, "insert") == 0) 
        {
            wData(name);
        } 
        else if (strcmp(op, "list") == 0 || strcmp(op, "show") == 0) 
        {
            lData();
        } 
        else if (strcmp(op, "rm") == 0 || strcmp(op, "remove") == 0 || strcmp(op, "Remove")== 0 || strcmp(op, "RM")== 0)
        {
            rmData(name);
        }
        
        else 
        {
            printf("Unknown command: %s\n", op);
        }
    }

    free(name);
    free(op);
    return 0;
}

void getinput(char line[], char op[]) 
{
    int c, i = 0, j = 0, op_done = 0;

    while ((c = getchar()) != '\n' && c != EOF) 
    {
        if (c == ' ' && !op_done)
        
        {
            op[i] = '\0'; 
            op_done = 1;  
            continue;     // Skip the space
        }

        if (!op_done) // op_done > 0
        {
            op[i++] = c; 
        } 
        else 
        {
            line[j++] = c; // Add to `line` after the space
        }

    }


    op[i] = '\0';  
    line[j] = '\0';
}


void wData(char data[])
{
    FILE *fl = fopen("Data/Airdrop.csv", "a");
   
    if (fl == NULL) {
        printf("\033[0;31mError: Could not open file.\033[0m\n");
        return;
    }

     fprintf(fl, "%s\n", data);
    
    fclose(fl);
    return;
}

void lData() {
    FILE *fl = fopen("Data/Airdrop.csv", "r");
    if (fl == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    char line[1000];
    int colorIndex = 0;

    printf("=== Airdrop LIST ===\n");

    // Loop through each line in the file
    while (fgets(line, sizeof(line), fl)) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline if present

        // Cycle through different colors
        switch (colorIndex % 5) {
            case 0: printf("\033[0;31m%s\033[0m\n", line); break; // Red
            case 1: printf("\033[0;33m%s\033[0m\n", line); break; // Yellow
            case 2: printf("\033[0;32m%s\033[0m\n", line); break; // Green
            case 3: printf("\033[0;34m%s\033[0m\n", line); break; // Blue
            case 4: printf("\033[0;35m%s\033[0m\n", line); break; // Magenta
        }

        colorIndex++; // Move to the next color
    }

    fclose(fl);
    printf("=== End of Airdrop List ===\n");
}


void rmData(char targetContent[])
{
      FILE *file = fopen("Data/Airdrop.csv", "r+");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *temp = tmpfile(); // Temporary in-memory file
    if (temp == NULL) {
        perror("Error creating temporary file");
        fclose(file);
        return;
    }

    char buffer[1024];
    int lineRemoved = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newline character for comparison
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, targetContent) == 0) {
            lineRemoved = 1; // Mark that we removed the line
            continue; // Skip this line
        }

        fprintf(temp, "%s\n", buffer); // Write other lines to temp
    }

    if (lineRemoved) 
    {
        // Truncate the original file and rewind to overwrite
        ftruncate(fileno(file), 0);
        rewind(file);
        rewind(temp);

        // Copy the temporary file back to the original file
        while (fgets(buffer, sizeof(buffer), temp)) 
        {
            fputs(buffer, file);
        }
        printf("Line '%s' removed successfully.\n", targetContent);
    } 
    else 
    {
        printf("Line '%s' not found in the file.\n", targetContent);
    }

    fclose(temp);
    fclose(file);
}
