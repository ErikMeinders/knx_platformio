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
