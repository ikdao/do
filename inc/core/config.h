// include/core/config.h
#ifndef DO_CONFIG_H
#define DO_CONFIG_H

#include <json-c/json.h>
#include <stdbool.h>
#include <stdint.h>

// Standardized size limits
#define MAX_HOSTNAME_LEN 255
#define MAX_PATH_LEN 4096
#define MAX_STRING_LEN 256
#define MAX_ARRAY_SIZE 100
#define MAX_FILE_EXTENSIONS 20
#define MAX_ENV_VARS 20
#define MAX_CIPHER_SUITES 30
#define MAX_CORS_HEADERS 30
#define MAX_CORS_ORIGINS 20
#define MAX_CORS_METHODS 10

// Core resource limits
typedef struct {
    char max_memory[MAX_STRING_LEN];        // "2GB", "512MB", etc.
    char max_heap[MAX_STRING_LEN];          // Language-specific heap limit
    size_t max_stack_size;                  // Stack size per request
    
    int max_connections;                    // Total concurrent connections
    int max_connections_per_ip;             // Per-IP connection limit
    int max_requests_per_conn;              // Requests per keep-alive connection
    
    char max_bandwidth[MAX_STRING_LEN];     // "1Gbps", "100Mbps", etc.
    char max_upload_rate[MAX_STRING_LEN];   // "10MB/s", etc.
    char max_download_rate[MAX_STRING_LEN]; // "50MB/s", etc.
    
    char max_file_size[MAX_STRING_LEN];     // "100MB" upload limit
    int max_open_files;                     // Open file descriptors
    int max_directory_depth;                // Max path depth for security
    
    int request_timeout;                    // Seconds
    int idle_timeout;                       // Seconds for idle connections
    int startup_timeout;                    // Seconds for app startup
    
    int requests_per_second;                // Global rate limit
    int requests_per_minute;                // Per-tenant rate limit
} limits_config_t;

// Security configuration
typedef struct {
    // Filesystem permissions
    char fs_read[MAX_ARRAY_SIZE][MAX_PATH_LEN];
    int fs_read_count;
    char fs_write[MAX_ARRAY_SIZE][MAX_PATH_LEN];
    int fs_write_count;
    char fs_execute[MAX_ARRAY_SIZE][MAX_PATH_LEN];
    int fs_execute_count;
    bool fs_deny_dotfiles;                  // Block .env, .git, etc.
    
    // Network permissions
    char net_outbound[MAX_ARRAY_SIZE][MAX_STRING_LEN]; // "host:port"
    int net_outbound_count;
    char net_inbound[MAX_ARRAY_SIZE][MAX_STRING_LEN];  // "ip:port"
    int net_inbound_count;
    bool net_block_private_ips;             // Block RFC1918 addresses
    
    // Environment permissions
    char env_allow[MAX_ARRAY_SIZE][MAX_STRING_LEN];
    int env_allow_count;
    char env_deny[MAX_ARRAY_SIZE][MAX_STRING_LEN];
    int env_deny_count;
    bool env_block_all;                     // Block all env vars by default
    
    // HTTP security headers
    struct {
        bool enabled;
        char content_security_policy[MAX_STRING_LEN];
        char x_frame_options[MAX_STRING_LEN];
        char x_content_type_options[MAX_STRING_LEN];
        char referrer_policy[MAX_STRING_LEN];
        char strict_transport_security[MAX_STRING_LEN];
        bool x_xss_protection;
    } security_headers;
    
    // Authentication
    struct {
        bool enabled;
        char type[32];                      // "basic", "jwt", "oauth2"
        char realm[128];                    // Basic auth realm
        char jwt_issuer[256];               // JWT issuer
        char jwt_audience[256];             // JWT audience
        char public_key[MAX_PATH_LEN];      // JWT public key path
        bool require_https;                 // Force HTTPS for auth
    } auth;
    
    // TLS/SSL settings
    struct {
        bool require_https;                 // Redirect HTTP to HTTPS
        char min_tls_version[16];           // "1.2", "1.3"
        char cipher_suites[MAX_CIPHER_SUITES][64]; // Allowed cipher suites
        int cipher_suites_count;
        bool prefer_server_ciphers;         // Prefer server cipher order
        bool ocsp_stapling;                 // Enable OCSP stapling
    } tls;
    
    // CORS settings
    struct {
        bool enabled;
        char allowed_origins[MAX_CORS_ORIGINS][256]; // CORS allowed origins
        int allowed_origins_count;
        char allowed_methods[MAX_CORS_METHODS][16];  // CORS allowed methods
        int allowed_methods_count;
        char allowed_headers[MAX_CORS_HEADERS][128]; // CORS allowed headers
        int allowed_headers_count;
        bool allow_credentials;             // CORS allow credentials
        int max_age;                        // CORS max age in seconds
    } cors;
    
    // Content security
    struct {
        char allowed_file_extensions[MAX_FILE_EXTENSIONS][16]; // Allowed extensions
        int allowed_file_extensions_count;
        char denied_file_extensions[MAX_FILE_EXTENSIONS][16];  // Blocked extensions
        int denied_file_extensions_count;
    } content;
} security_config_t;

// Server configuration
typedef struct {
    int port;
    char host[MAX_HOSTNAME_LEN + 1];        // Bind address
    char acme_email[256];                   // Email for Let's Encrypt
    bool auto_https;                        // Auto redirect to HTTPS
    char server_name[128];                  // Server header value
    bool hide_server_header;                // Hide server version
    int worker_processes;                   // Number of worker processes
    int worker_threads;                     // Threads per worker
    bool enable_http2;                      // Enable HTTP/2
    bool enable_http3;                      // Enable HTTP/3
    bool enable_websockets;                 // Enable WebSocket support
    bool enable_webtransport;               // Enable WebTransport support
} server_config_t;

// Virtual host configuration
typedef struct {
    char name[MAX_HOSTNAME_LEN + 1];        // Hostname
    char docroot[MAX_PATH_LEN];             // Document root path
    int port;                               // Port to listen on
    char bind_address[MAX_HOSTNAME_LEN + 1]; // Specific IP to bind to
    
    struct {
        char cert[MAX_PATH_LEN];            // Certificate path
        char key[MAX_PATH_LEN];             // Private key path
        char ca_cert[MAX_PATH_LEN];         // CA certificate path
        bool auto_cert;                     // Auto-generate certificate
    } tls;
    
    bool http1;                             // Enable HTTP/1.1
    bool http2;                             // Enable HTTP/2
    bool http3;                             // Enable HTTP/3
    
    // Protocol-specific settings
    struct {
        int max_concurrent_streams;         // HTTP/2 max streams
        int initial_window_size;            // HTTP/2 flow control
    } http2_settings;
    
    struct {
        int max_webtransport_sessions;      // WebTransport sessions
        bool datagram_enabled;              // WebTransport datagrams
    } webtransport_settings;
    
    // Tenant-specific limits and security
    limits_config_t limits;
    security_config_t security;
    
    // Language runtime settings
    struct {
        char default_language[16];          // "js", "lua", "python"
        bool enable_jit;                    // Enable JIT compilation
        int timeout;                        // Script execution timeout
        bool sandbox_enabled;               // Enable language sandbox
    } runtime;
} vhost_config_t;

// Cron job configuration
typedef struct {
    char schedule[64];                      // Cron schedule ("0 0 * * *")
    char script[MAX_PATH_LEN];              // Script path to execute
    char language[16];                      // Language to use ("js", "lua", etc.)
    char working_directory[MAX_PATH_LEN];   // Working directory for execution
    char environment[MAX_ENV_VARS][256];    // Environment variables for job
    int environment_count;
    int timeout;                            // Job timeout in seconds
    bool enabled;                           // Whether job is enabled
} cron_job_t;

// Plugin configuration
typedef struct {
    char name[64];                          // Plugin name
    char path[MAX_PATH_LEN];                // Plugin file path (.so, .dll, .wasm)
    bool enabled;                           // Whether plugin is enabled
    json_object* config;                    // Plugin-specific configuration
} plugin_config_t;

// Enhanced logging configuration
typedef struct {
    char level[32];                         // "debug", "info", "warn", "error"
    char format[32];                        // Log format ("json", "text")
    char file[MAX_PATH_LEN];                // Log file path (null for stdout)
    bool rotate;                            // Enable log rotation
    char max_size[MAX_STRING_LEN];          // Max log file size ("100MB")
    int max_files;                          // Max rotated files to keep
    bool include_timestamp;                 // Include timestamp in logs
    bool include_request_id;                // Include request ID in logs
} logging_config_t;

// Monitoring configuration
typedef struct {
    bool enabled;                           // Enable monitoring
    char endpoint[64];                      // Metrics endpoint path ("/metrics")
    char format[32];                        // Metrics format ("prometheus", "json")
    bool include_runtime_stats;             // Include language runtime stats
    bool include_system_stats;              // Include system stats (CPU, memory)
    int collection_interval;                // Stats collection interval (seconds)
} monitoring_config_t;

// Cache configuration
typedef struct {
    bool enabled;                           // Enable caching
    char backend[32];                       // Cache backend ("memory", "redis", "file")
    char ttl[32];                           // Default TTL ("1h", "30m", etc.)
    char max_size[MAX_STRING_LEN];          // Max cache size ("1GB", "512MB")
    bool compress;                          // Compress cached content
    struct {
        char paths[50][MAX_PATH_LEN];       // Paths to cache (glob patterns)
        int paths_count;
        char extensions[50][16];            // File extensions to cache
        int extensions_count;
        bool cache_dynamic;                 // Cache dynamic responses
    } rules;
} cache_config_t;

// Main enhanced configuration structure
typedef struct {
    server_config_t server;
    limits_config_t limits;
    security_config_t security;
    logging_config_t logging;
    monitoring_config_t monitoring;
    cache_config_t cache;
    
    vhost_config_t vhosts[MAX_ARRAY_SIZE];
    int vhost_count;
    
    cron_job_t cron[20];                    // Limited cron jobs
    int cron_count;
    
    plugin_config_t plugins[10];            // Limited plugins
    int plugin_count;
    
    char includes[MAX_ARRAY_SIZE][MAX_PATH_LEN]; // Include patterns
    int includes_count;
    
    // Global settings
    char timezone[64];                      // Default timezone
    char locale[32];                        // Default locale
    bool debug_mode;                        // Enable debug mode
    char data_directory[MAX_PATH_LEN];      // Directory for runtime data
    char temp_directory[MAX_PATH_LEN];      // Directory for temporary files
} do_config_t;

extern do_config_t g_config;

// Function declarations
int config_load(const char* path);
void config_free(do_config_t* config);
char* config_interpolate_env(const char* input);
const vhost_config_t* config_get_vhost_by_name(const char* hostname);
const vhost_config_t* config_get_default_vhost(void);

#endif