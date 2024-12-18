#!/bin/bash

afficher_arborescence() {
    local dir="$1"
    local prefix="$2"
    local items=()
    local count=0

    # Récupérer uniquement les répertoires
    for item in "$dir"/*; do
        if [ -d "$item" ]; then
            items+=("$item")
            ((count++))
        fi
    done

    local i=0

    for item in "${items[@]}"; do
        ((i++))
        local nom=$(basename "$item")
        if [ $i -lt $count ]; then
            echo "${prefix}├── $nom"
            afficher_arborescence "$item" "${prefix}│   "
        else
            echo "${prefix}└── $nom"
            afficher_arborescence "$item" "${prefix}    "
        fi
    done
}

# Point de départ
echo "."
afficher_arborescence "." ""
