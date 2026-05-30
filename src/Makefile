# Makefile — Reverse Engineering Lab
# Builds both the original and modified versions of the reconstructed binary
#
# Usage:
#   make all        → build both versions
#   make original   → build original string version  (expects: 15015)
#   make modified   → build modified string version  (expects: 26080)
#   make run        → build and run both
#   make clean      → remove compiled binaries

CC      = gcc
CFLAGS  = -m32 -Wall -Wextra

.PHONY: all original modified run clean check-multilib

all: check-multilib original modified

check-multilib:
	@which gcc >/dev/null 2>&1 || (echo "[ERROR] gcc not found. Install with: sudo apt install gcc" && exit 1)
	@echo "int main(){return 0;}" | $(CC) $(CFLAGS) -x c - -o /tmp/test_m32 2>/dev/null || \
		(echo "[ERROR] 32-bit compilation not supported." && \
		 echo "        Fix with: sudo apt install gcc-multilib" && exit 1)
	@rm -f /tmp/test_m32
	@echo "[OK] 32-bit gcc support confirmed."

original: original/source.c
	@echo "[BUILD] Compiling original version..."
	$(CC) $(CFLAGS) original/source.c -o original/surca
	@echo "[DONE]  Binary: original/surca"

modified: modified/source_congratulations.c
	@echo "[BUILD] Compiling modified version (Congratulations!)..."
	$(CC) $(CFLAGS) modified/source_congratulations.c -o modified/surca_congratulations
	@echo "[DONE]  Binary: modified/surca_congratulations"

run: all
	@echo ""
	@echo "════════════════════════════════════════"
	@echo " Running ORIGINAL (expected: 15015)"
	@echo "════════════════════════════════════════"
	./original/surca
	@echo ""
	@echo "════════════════════════════════════════"
	@echo " Running MODIFIED (expected: 26080)"
	@echo "════════════════════════════════════════"
	./modified/surca_congratulations

clean:
	@rm -f original/surca modified/surca_congratulations
	@echo "[CLEAN] Binaries removed."
