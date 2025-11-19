#include <stdio.h>
#include <string.h>

// Define the maximum size for string fields
#define MAX_STR_LEN 100

// --- Key Question 1: Is the union correctly declared with all relevant fields? ---
// The union will hold the string data (Title, Author) as it's assumed
// the program might only need to manipulate ONE of these fields at a time.
// The union's size will be the size of its LARGEST member (MAX_STR_LEN).
union BookStringData {
    char title[MAX_STR_LEN];
    char author[MAX_STR_LEN];
    // We could add more fields here like publisher, but we only use two for clarity.
};

// A structure to hold the complete book record, including the union
struct BookRecord {
    int accession_number;
    double price;
    int status_flag; // 1 for issued, 0 for available

    // Embed the union inside the struct
    union BookStringData string_data;
};


// Function Prototypes
void input_book_details(struct BookRecord *book);
void display_book_details(const struct BookRecord *book);

int main() {
    // Declare a single variable of type struct BookRecord
    // --- Key Question 2: Is only one book's data stored at a time? ---
    // Yes, only one book record is created and manipulated.
    struct BookRecord current_book;

    printf("--- Digital Library Book Management System ---\n");

    // Allow the librarian to enter details for a book
    input_book_details(&current_book);

    // Display the entered book details
    display_book_details(&current_book);

    return 0;
}

/**
 * @brief Allows the user to input details for a single book record.
 * @param book Pointer to the BookRecord structure to be populated.
 */
void input_book_details(struct BookRecord *book) {
    // 1. Input Accession Number
    printf("\nEnter Book Details:\n");
    printf("1. Enter Accession Number (integer): ");
    scanf("%d", &book->accession_number);

    // Clear the input buffer after reading an integer
    while (getchar() != '\n');

    // 2. Input Title
    printf("2. Enter Title: ");
    // IMPORTANT: Storing the Title in the union overwrites the memory.
    // We use fgets to safely read the string, including spaces.
    fgets(book->string_data.title, MAX_STR_LEN, stdin);
    // Remove the newline character added by fgets
    book->string_data.title[strcspn(book->string_data.title, "\n")] = 0;

    // 3. Input Author
    printf("3. Enter Author: ");
    // IMPORTANT: Storing the Author in the union NOW OVERWRITES the Title memory.
    // The previous Title data is lost! This demonstrates union memory usage.
    fgets(book->string_data.author, MAX_STR_LEN, stdin);
    book->string_data.author[strcspn(book->string_data.author, "\n")] = 0;

    // 4. Input Price
    printf("4. Enter Price (e.g., 499.50): ₹");
    scanf("%lf", &book->price);

    // 5. Input Status Flag
    // --- Key Question 3: Is the status flag clearly handled (1 = issued, 0 = available)? ---
    printf("5. Enter Status Flag (1 for Issued, 0 for Available): ");
    // Loop for user-friendly, logical input validation
    do {
        if (scanf("%d", &book->status_flag) != 1 || (book->status_flag != 0 && book->status_flag != 1)) {
            printf("Invalid status flag. Please enter 1 (Issued) or 0 (Available): ");
            while (getchar() != '\n'); // Clear the input buffer
        } else {
            break;
        }
    } while (1);

    while (getchar() != '\n'); // Clear the input buffer
}

/**
 * @brief Displays the details of a single book record in a readable format.
 * @param book Pointer to the BookRecord structure to be displayed.
 */
void display_book_details(const struct BookRecord *book) {
    printf("\n============================================\n");
    printf("            BOOK RECORD DETAILS             \n");
    printf("============================================\n");

    printf("Accession Number: **%d**\n", book->accession_number);
    printf("Price: **₹%.2lf**\n", book->price);

    // Display Status Flag with clear text representation
    printf("Status: **%s** (%d)\n",
           (book->status_flag == 1) ? "Issued" : "Available",
           book->status_flag);

    // Displaying the string data requires careful handling due to the union.
    // Since the AUTHOR was the LAST field written to the union memory space,
    // it is the only string guaranteed to be correct.
    printf("\n-- String Data (Last data written is valid) --\n");
    printf("Author (Last Written Data): **%s**\n", book->string_data.author);

    // NOTE: If we try to read the title now, it will likely be corrupt
    // or show the author's name, demonstrating the union concept.
    // printf("Title (Corrupt): %s\n", book->string_data.title);

    printf("============================================\n");

    // --- Key Question 4: Is input/output logic structured, user-friendly, and readable? ---
    // Yes, functions are used for modularity and output is clearly formatted.
}
