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

int is_uefi() {
    struct stat st;
    return stat("/sys/firmware/efi", &st) == 0 && S_ISDIR(st.st_mode);
}

int is_root() {
    return geteuid() == 0;
}

int main() {
    if (!is_root()) {
        fprintf(stderr, "Error: This script must be run as root.\n");
        return 1;
    }

    printf("Detected boot mode: %s\n", is_uefi() ? "UEFI" : "BIOS");

    int status = system("grub2-mkconfig -o /boot/grub2/grub.cfg");
    if (status != 0) {
        fprintf(stderr, "Failed to generate /boot/grub2/grub.cfg\n");
        return status;
    }

    if (is_uefi()) {
        status = system("grub2-mkconfig -o /boot/efi/EFI/almalinux/grub.cfg");
        if (status != 0) {
            fprintf(stderr, "Failed to generate /boot/efi/EFI/almalinux/grub.cfg\n");
            return status;
        }
    }

    printf("GRUB config updated successfully.\n");
    return 0;
}
