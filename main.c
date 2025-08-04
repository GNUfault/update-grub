/*
 * update-grub -  `update-grub` command for distros that don't come with it.
 * Copyright (C) 2025  Connor Thomson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

int is_uefi() {
    struct stat st;
    return stat("/sys/firmware/efi", &st) == 0 && S_ISDIR(st.st_mode);
}

int is_root() {
    return geteuid() == 0;
}

const char* detect_distro_name() {
    FILE *fp = fopen("/etc/os-release", "r");
    if (!fp) return "unknown";

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "ID=", 3) == 0) {
            fclose(fp);
            if (strstr(line, "fedora")) return "Fedora";
            if (strstr(line, "almalinux")) return "AlmaLinux";
            if (strstr(line, "rocky")) return "Rocky Linux";
            return "Other";
        }
    }

    fclose(fp);
    return "unknown";
}

int main() {
    if (!is_root()) {
        fprintf(stderr, "This tool must be run as root.\n");
        return 1;
    }

    printf("Boot mode: %s\n", is_uefi() ? "UEFI" : "BIOS");

    int status = system("grub2-mkconfig -o /boot/grub2/grub.cfg");
    if (status != 0) {
        fprintf(stderr, "Failed to generate /boot/grub2/grub.cfg\n");
        return status;
    }

    if (is_uefi()) {
        const char* efi_path = detect_distro_efi_path();
        printf("Detected EFI target: %s\n", efi_path);
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "grub2-mkconfig -o %s", efi_path);
        status = system(cmd);
        if (status != 0) {
            fprintf(stderr, "Failed to generate %s\n", efi_path);
            return status;
        }
    }

    printf("GRUB config updated successfully.\n");
    return 0;
}
