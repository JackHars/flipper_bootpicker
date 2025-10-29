#include "profiles.h"
#include <furi.h>
#include <storage/storage.h>
#include <toolbox/stream/file_stream.h>

#define TAG "BootPicker:Profiles"

// Dynamic profiles array
Profile PROFILES[MAX_PROFILE_COUNT];
uint8_t profile_count = 0;

// Simple JSON parser for our specific format
static bool parse_json_profiles(const char* json_data, size_t size) {
    if(!json_data || size == 0) {
        return false;
    }
    
    profile_count = 0;
    
    // Find the "profiles" array
    const char* profiles_start = strstr(json_data, "\"profiles\"");
    if(!profiles_start) {
        FURI_LOG_E(TAG, "No 'profiles' array found in JSON");
        return false;
    }
    
    // Find opening bracket of profiles array
    const char* array_start = strchr(profiles_start, '[');
    if(!array_start) {
        FURI_LOG_E(TAG, "No opening bracket for profiles array");
        return false;
    }
    
    const char* pos = array_start + 1;
    const char* end = json_data + size;
    
    // Parse each profile object
    while(pos < end && profile_count < MAX_PROFILE_COUNT) {
        // Find next opening brace for profile object
        const char* obj_start = strchr(pos, '{');
        if(!obj_start || obj_start >= end) break;
        
        // Check if we've reached the end of the array
        const char* array_end = strchr(pos, ']');
        if(array_end && array_end < obj_start) break;
        
        pos = obj_start + 1;
        
        // Parse "name" field
        const char* name_key = strstr(pos, "\"name\"");
        if(!name_key || name_key >= end) break;
        
        const char* name_colon = strchr(name_key, ':');
        if(!name_colon || name_colon >= end) break;
        
        const char* name_quote = strchr(name_colon, '"');
        if(!name_quote || name_quote >= end) break;
        
        const char* name_start = name_quote + 1;
        const char* name_end = strchr(name_start, '"');
        if(!name_end || name_end >= end) break;
        
        // Copy name
        size_t name_len = name_end - name_start;
        if(name_len >= MAX_PROFILE_NAME_LEN) {
            name_len = MAX_PROFILE_NAME_LEN - 1;
        }
        memcpy(PROFILES[profile_count].name, name_start, name_len);
        PROFILES[profile_count].name[name_len] = '\0';
        
        // Parse "arrow_count" field
        const char* arrow_key = strstr(name_end, "\"arrow_count\"");
        if(!arrow_key || arrow_key >= end) break;
        
        const char* arrow_colon = strchr(arrow_key, ':');
        if(!arrow_colon || arrow_colon >= end) break;
        
        // Skip whitespace after colon
        const char* arrow_value = arrow_colon + 1;
        while(*arrow_value == ' ' || *arrow_value == '\t' || *arrow_value == '\n' || 
              *arrow_value == '\r') {
            arrow_value++;
            if(arrow_value >= end) break;
        }
        
        // Parse number
        int arrow_count_val = 0;
        if(arrow_value < end && *arrow_value >= '0' && *arrow_value <= '9') {
            arrow_count_val = atoi(arrow_value);
        }
        
        PROFILES[profile_count].arrow_count = (uint8_t)arrow_count_val;
        
        FURI_LOG_I(
            TAG,
            "Loaded profile %d: name='%s', arrow_count=%d",
            profile_count,
            PROFILES[profile_count].name,
            PROFILES[profile_count].arrow_count);
        
        profile_count++;
        
        // Find closing brace of current object
        const char* obj_end = strchr(arrow_value, '}');
        if(!obj_end || obj_end >= end) break;
        pos = obj_end + 1;
    }
    
    FURI_LOG_I(TAG, "Loaded %d profiles from JSON", profile_count);
    return profile_count > 0;
}

bool profiles_load_from_file(const char* file_path) {
    FURI_LOG_I(TAG, "Loading profiles from: %s", file_path);
    
    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* stream = file_stream_alloc(storage);
    
    bool success = false;
    
    if(!file_stream_open(stream, file_path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FURI_LOG_E(TAG, "Failed to open profiles file: %s", file_path);
        
        // Load default profiles if file doesn't exist
        FURI_LOG_W(TAG, "Loading default profiles");
        profile_count = 9;
        
        snprintf(PROFILES[0].name, MAX_PROFILE_NAME_LEN, "Windows");
        PROFILES[0].arrow_count = 0;
        
        snprintf(PROFILES[1].name, MAX_PROFILE_NAME_LEN, "Ubuntu");
        PROFILES[1].arrow_count = 1;
        
        snprintf(PROFILES[2].name, MAX_PROFILE_NAME_LEN, "Profile 3");
        PROFILES[2].arrow_count = 2;
        
        snprintf(PROFILES[3].name, MAX_PROFILE_NAME_LEN, "Profile 4");
        PROFILES[3].arrow_count = 3;
        
        snprintf(PROFILES[4].name, MAX_PROFILE_NAME_LEN, "Profile 5");
        PROFILES[4].arrow_count = 4;
        
        snprintf(PROFILES[5].name, MAX_PROFILE_NAME_LEN, "Profile 6");
        PROFILES[5].arrow_count = 5;
        
        snprintf(PROFILES[6].name, MAX_PROFILE_NAME_LEN, "Profile 7");
        PROFILES[6].arrow_count = 6;
        
        snprintf(PROFILES[7].name, MAX_PROFILE_NAME_LEN, "Profile 8");
        PROFILES[7].arrow_count = 7;
        
        snprintf(PROFILES[8].name, MAX_PROFILE_NAME_LEN, "P3");
        PROFILES[8].arrow_count = 8;
        
        success = true;
        goto cleanup;
    }
    
    size_t file_size = stream_size(stream);
    if(file_size == 0 || file_size > 4096) { // Limit to 4KB
        FURI_LOG_E(TAG, "Invalid file size: %zu", file_size);
        goto cleanup;
    }
    
    char* buffer = malloc(file_size + 1);
    if(!buffer) {
        FURI_LOG_E(TAG, "Failed to allocate buffer");
        goto cleanup;
    }
    
    size_t bytes_read = stream_read(stream, (uint8_t*)buffer, file_size);
    if(bytes_read != file_size) {
        FURI_LOG_E(TAG, "Failed to read file: read %zu of %zu bytes", bytes_read, file_size);
        free(buffer);
        goto cleanup;
    }
    
    buffer[file_size] = '\0';
    
    FURI_LOG_D(TAG, "Read %zu bytes from file", file_size);
    
    success = parse_json_profiles(buffer, file_size);
    
    free(buffer);
    
cleanup:
    file_stream_close(stream);
    stream_free(stream);
    furi_record_close(RECORD_STORAGE);
    
    return success;
}

const Profile* profile_get(uint8_t index) {
    if(index >= profile_count) {
        return NULL;
    }
    return &PROFILES[index];
}

uint8_t profiles_get_count(void) {
    return profile_count;
}
