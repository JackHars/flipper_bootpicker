#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAX_PROFILE_COUNT 9
#define MAX_PROFILE_NAME_LEN 32

// Profile structure
typedef struct {
    char name[MAX_PROFILE_NAME_LEN];
    uint8_t arrow_count;  // 0-8 (number of DOWN arrows before ENTER)
} Profile;

// Dynamic profiles array
extern Profile PROFILES[MAX_PROFILE_COUNT];
extern uint8_t profile_count;

// Load profiles from JSON file
bool profiles_load_from_file(const char* file_path);

// Get profile by index
const Profile* profile_get(uint8_t index);

// Get total number of loaded profiles
uint8_t profiles_get_count(void);
