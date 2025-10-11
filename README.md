# DO Server+runner — Minimal, Secure, Multi-Tenant Server + Language Runtime

**DO** is a **single-binary**, **Cross platform** **language-agnostic**, **multi-tenant** **orchestration ready** server+runner, currently in development that merges a **high-performance web server** with **embedded language runner** with **zero external dependencies**, and **enterprise-grade security**, all under 5MB, mimimal over existence and events but performance and possibilities

Designed for modern edge, serverless, sustainable and hosting/paas/SaaS platforms, DO lets you run JavaScript, WebAssembly with other languages pluggable safely, efficiently, and without bloat.

---

## ✨ Features

- **Single binary, <5MB** — statically linked, no runtime deps
- **Multi-language support**:
  - JavaScript/TypeScript (via **QuickJS** deciding)
  - WebAssembly (via **WAMR**)
  - (Future: mruby, MicroPython)
- **True multi-tenancy**:
  - Per-tenant filesystem isolation
  - Resource limits (memory, connections, bandwidth)
  - Path traversal protection
- **Modern protocols**:
  - HTTP/1.1, HTTP/2, **HTTP/3 (QUIC)**
  - WebSockets & **WebTransport-ready**
  - TLS 1.3 with **automatic Let’s Encrypt (ACME) or Self signed**
- **Secure by default**:
  - Permission system (FS, network, env)
  - Automatic security headers (HSTS, CSP, X-Frame-Options)
  - Rate limiting, CORS, JWT auth
- **Developer-friendly**:
  - Hot config reload
  - Local TLS + `.local` aliases
  - Structured logging (JSON) metrics
- **Cross-platform**:
  - Linux (`io_uring`/`epoll`)
  - Windows (`IOCP`)
  - macOS (`kqueue`/GCD)

---


## 🛡️ Security Model

DO enforces security at the **system boundary**, not in application code:
- All file/network access is mediated by DO’s permission layer
- Tenants cannot escape their `docroot`
- Language runtimes run in restricted sandboxes
- No unsafe defaults — `dev_mode` must be explicitly enabled

> DO is built for **zero-trust environments** — ideal for PaaS, edge hosting, and multi-tenant SaaS.

---

## 📦 Architecture

```
do/
├── lib/          # json-c, QuickJS, WAMR, BoringSSL, nghttp2, lsquic
├── inc/       # Public C API
├── src/           # Core, sourcer, system, security, server, supply
├── cfg/        # Example configs
└── application/      # JS/WASM apps
```

- **C-only codebase** — simple, auditable, fast
- **Pluggable language ABI** — add runtimes without touching core
- **Configuration-driven** — no code changes needed for most behaviors

---

## 📜 License

DO is licensed under the **Self License** — a minimal, human-readable licensing standard created by [ikdao](https://ikdao.org).

- **0SL (Zero Self License)**: No terms, public domain–like freedom.
- **1SL (One Self License)**: Terms apply; see license text.

Learn more:  
🔗 [https://legal.ikdao.org/license/](https://legal.ikdao.org/license/)

---

## 🤝 Contributing

We welcome contributions that align with DO’s values yet to decide:
- **Sustainable**
- **Security**
- **Performance**
- **Portability**

Please open an issue or PR on GitHub.

---

## 🌐 Links

-  [Requirements Spec](docs/requirements.md)
-  [Twitter](https://twitter.com/ikdao)
-  [Facebook](https://facebook.com/ikdao)

---

**DO** — *Do more with less. Run anything. Trust nothing. Grow Autonomy*