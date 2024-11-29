# AI Assistant Learnings

## Project Environment
1. PlatformIO CLI Usage:
   - Must activate virtual environment before using PlatformIO tools
   - Use `source ~/.platformio/penv/bin/activate` to activate
   - After activation, use `pio` command (not `platformio`)
   - Each new command runs in a new shell, so activation needs to be combined with the command using `&&`

## Project Documentation
1. Always read `ai/general.md` first for project context and requirements
2. Journal entries:
   - Must be created with current date (use `date +%Y-%m-%d` to get correct date)
   - Store in `journal/` directory with format `YYYY-MM-DD.md`
   - Include detailed information about tasks, steps, and results

## Development Process
1. Before making changes:
   - Read project documentation first
   - Understand the project structure
   - Check existing configurations
2. After making changes:
   - Document results and learnings
   - Update documentation when necessary

## Common Pitfalls to Avoid
1. Don't assume the date for journal entries - always get current date from system
2. Don't use `platformio` command - use `pio` after activation
3. Don't forget to combine virtual environment activation with commands using `&&`
4. Don't proceed without reading project documentation first

## Project-Specific Insights
1. Memory Usage Patterns:
   - ESP8266 is more constrained in RAM (45.5% usage) but efficient in flash usage (48.2%)
   - ESP32 has better RAM headroom (15.7%) but higher flash usage (82.5%)
   - These baselines are important for future optimizations

2. Build Configuration:
   - Project supports both ESP8266 and ESP32 platforms
   - Uses debug build type for both platforms
   - Includes important KNX-specific build flags (MASK_VERSION, feature flags)
   - Dependencies are properly managed through lib_deps

3. Library Structure:
   - Main library code in src/ and include/
   - Examples in examples/basic/
   - Basic example serves as a good test case for library functionality
   - Library version is currently at 0.1.0 (from compilation output)

4. Platform-Specific Dependencies:
   - ESP8266 requires explicit ESP8266WiFi library
   - ESP32 requires WiFi library
   - WebSocket implementation needs different headers per platform
   - LittleFS configuration varies between platforms

5. WebSocket Implementation:
   - ArduinoWebsockets library requires platform-specific WiFi headers
   - Client management needs careful handling with move semantics
   - Real-time updates require proper connection state tracking
   - Separate concerns between web server and WebSocket functionality
