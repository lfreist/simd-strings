/**
 * Copyright 2024, Leon Freist (https://github.com/lfreist)
 * Author: Leon Freist <freist.leon@gmail.com>
 * 
 * This file is part of simd_string.
 */

#include "minunit.h"

#include <assert.h>

#include <simdstr/slim_teddy.h>

char haystack[1024] = "The quick brown fox jumps over the lazy dog. Coding is a fascinating skill that requires dedication and practice. "
                      "Machine learning and artificial intelligence are revolutionizing the world. Cats and dogs make wonderful pets. The sun sets behind the mountains, "
                      "casting a warm glow over the valley. In winter, people enjoy skiing and snowboarding. The Internet has transformed how we communicate and share information. "
                      "A delicious pizza with pepperoni and cheese is my favorite. Playing the guitar is a great way to relax and express creativity. The moonlight shines brightly, "
                      "creating a magical atmosphere. Science and mathematics are fundamental to understanding the natural world. Security and privacy are essential in the digital age. "
                      "I enjoy reading novels and poetry in my free time. The colors of the rainbow are mesmerizing. Football and soccer are popular sports worldwide. "
                      "Computer keyboards and mice are essential for everyday tasks. I would not like to add Machine. This is my idea. But anyways, you go as you are";

char* str_patterns[255] = {
        "abcd", "efgh", "ijklm", "nopqr", "stuvw", "xyz", "1234", "5678", "90ab", "cdef",
        "GHIJKL", "MNOPQR", "STUVWX", "YZ", "apple", "banana", "cherry", "date", "grape", "kiwi",
        "LEMON", "orange", "peach", "plum", "tomato", "Carrot", "potato", "Onion", "Garlic", "Cabbage",
        "XYZ123", "456UVW", "789ABC", "DEF", "catDog", "fishBowl", "BirdCage", "Hamster", "Lizard", "Snake",
        "Rainbow", "Sunshine", "Moonlight", "Star", "Mountain", "River", "Ocean", "Desert", "Forest", "Meadow",
        "Coding", "Programming", "Debugging", "Algorithm", "Variable", "Function", "Pointer", "Array", "Loop", "Condition",
        "idea", "Machine", "Learning", "Artificial", "Intelligence", "Data", "Science", "Algorithm", "Computer", "Technology",
        "Coffee", "Tea", "Espresso", "Latte", "Cappuccino", "Mocha", "Java", "Cocoa", "Scone", "Croissant",
        "Guitar", "Piano", "Violin", "Trumpet", "Flute", "Drums", "Saxophone", "Cello", "Harmonica", "Accordion",
        "Red", "Blue", "Green", "Yellow", "Purple", "Orange", "Pink", "Brown", "Black", "White",
        "Winter", "Spring", "Summer", "Autumn", "January", "July", "December", "Friday", "Monday", "Wednesday",
        "Mountain", "Valley", "Canyon", "Plain", "Peak", "Hill", "Island", "Cave", "Lake", "Waterfall",
        "Mars", "Jupiter", "Saturn", "Venus", "Neptune", "Mercury", "Uranus", "Pluto", "Earth", "Sun",
        "Butterfly", "Dragonfly", "Ladybug", "Bee", "Ant", "Spider", "Moth", "Caterpillar", "Snail", "Beetle",
        "Candle", "Lantern", "Flashlight", "Bonfire", "Fireplace", "Incense", "Torch", "Headlamp", "Spotlight", "Glow",
        "Internet", "Browser", "Website", "Server", "Email", "Password", "Firewall", "Encryption", "Download", "Upload",
        "Hamburger", "Pizza", "Hotdog", "Sandwich", "Pasta", "Salad", "Taco", "Burrito", "Sushi", "Soup",
        "Book", "Novel", "Magazine", "Newspaper", "Journal", "Story", "Poem", "Fiction", "Nonfiction", "Sci-fi",
        "Camera", "Photograph", "Lens", "Shutter", "Flash", "Tripod", "Zoom", "Focus", "Aperture", "Capture",
        "Painting", "Sculpture", "Drawing", "Sketch", "Canvas", "Palette", "Brush", "Easel", "Palette", "Abstract",
        "Football", "Soccer", "Basketball", "Tennis", "Golf", "Baseball", "Volleyball", "Hockey", "Rugby", "Cricket",
        "Train", "Bus", "Car", "Bicycle", "Motorcycle", "Subway", "Taxi", "Airplane", "Boat", "Ship",
        "Science", "Mathematics", "Physics", "Chemistry", "Biology", "Geology", "Astronomy", "Psychology", "Sociology", "Anthropology",
        "Cryptography", "Security", "Privacy", "Authentication", "Authorization", "Access", "Permission", "Validation", "Verification", "Credential", "Biometrics",
        "Ubuntu", "Fedora", "Debian", "CentOS", "Arch", "Mint", "Gentoo", "Slackware", "OpenSUSE", "FreeBSD",
        "Keyboard", "Mouse", "Monitor", "CPU"
};

SlimTeddy*
get_teddy (uint8_t num_patterns, uint8_t num_masks)
{
        assert (0 < num_masks && num_masks <= 4);

        Pattern* patterns = malloc (num_patterns * sizeof(Pattern));

        for (uint8_t pidx = 0; pidx < num_patterns; ++pidx)
        {
                patterns[pidx].begin = str_patterns[pidx];
                patterns[pidx].size = strlen(str_patterns[pidx]);
        }

        SlimTeddy* teddy = malloc(sizeof(SlimTeddy));

        SlimTeddy_init (teddy, patterns, num_patterns, num_masks);

        return teddy;
}

void free_teddy(SlimTeddy* teddy)
{
        free(teddy->patterns);
        free(teddy);
}

MU_TEST (find_1_test)
{
        SlimTeddy* teddy = get_teddy(4, 1);

        Match match = SlimTeddy_find_1 (teddy, haystack, 1024);

        free_teddy (teddy);

        mu_assert_int_eq (10, match.begin - haystack);
}

MU_TEST_SUITE (SlimTeddy_test)
{
        MU_RUN_TEST (find_1_test);
}

int main(int argc, char *argv[]) {
        MU_RUN_SUITE(SlimTeddy_test);
        MU_REPORT();
        return MU_EXIT_CODE;
}
