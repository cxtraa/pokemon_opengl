import requests
import json
import time

# Output JSON file
OUTPUT_FILE = "gen2_pokemon_crystal.json"

# Gen 2 Pokémon IDs (1-251 includes Kanto + Johto)
POKEMON_IDS = list(range(1, 252))

# Version group for Crystal
VERSION_GROUP = "crystal"

# Store all Pokémon data in a dictionary keyed by name
all_pokemon = {}

for poke_id in POKEMON_IDS:
    url = f"https://pokeapi.co/api/v2/pokemon/{poke_id}/"
    response = requests.get(url)
    if response.status_code != 200:
        print(f"Failed to fetch Pokémon ID {poke_id}")
        continue

    data = response.json()

    # Basic info
    poke_entry = {
        "id": data["id"],
        "types": [t["type"]["name"] for t in data["types"]],
        "baseStats": {stat["stat"]["name"]: stat["base_stat"] for stat in data["stats"]},
        "learnsets": {
            "levelUp": [],
            "machine": []
        }
    }

    # Parse moves
    for move_entry in data["moves"]:
        for version_detail in move_entry["version_group_details"]:
            if version_detail["version_group"]["name"] != VERSION_GROUP:
                continue
            method = version_detail["move_learn_method"]["name"]
            move_name = move_entry["move"]["name"]
            if method == "level-up":
                level = version_detail["level_learned_at"]
                poke_entry["learnsets"]["levelUp"].append({
                    "move": move_name,
                    "level": level
                })
            elif method == "machine":
                poke_entry["learnsets"]["machine"].append(move_name)

    # Optional: sort moves by level
    poke_entry["learnsets"]["levelUp"].sort(key=lambda m: m["level"])

    # Add to dictionary keyed by Pokémon name
    all_pokemon[data["name"]] = poke_entry
    
    # Respect API rate limits
    time.sleep(0.1)  # 100ms pause

# Save to JSON
with open(OUTPUT_FILE, "w") as f:
    json.dump(all_pokemon, f, indent=2)

print(f"Saved Gen 2 Pokémon data to {OUTPUT_FILE}")