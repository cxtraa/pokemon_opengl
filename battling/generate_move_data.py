import requests
import json
import time

VERSION_GROUP = "crystal"
POKEMON_IDS = range(1, 252)  # Gen 1 + Gen 2 Pokémon

moves_dict = {}

# Step 1: Collect all moves from each Pokémon's learnset
for poke_id in POKEMON_IDS:
    url = f"https://pokeapi.co/api/v2/pokemon/{poke_id}/"
    response = requests.get(url)
    if response.status_code != 200:
        print(f"Failed to fetch Pokémon ID {poke_id}")
        continue
    data = response.json()

    for move_entry in data["moves"]:
        for version_detail in move_entry["version_group_details"]:
            if version_detail["version_group"]["name"] != VERSION_GROUP:
                continue

            move_name = move_entry["move"]["name"]

            # Add move if not already in dictionary
            if move_name not in moves_dict:
                move_data = requests.get(move_entry["move"]["url"]).json()

                # Short description
                description = None
                for entry in move_data.get("effect_entries", []):
                    if entry["language"]["name"] == "en":
                        description = entry.get("short_effect")
                        break

                moves_dict[move_name] = {
                    "type": move_data["type"]["name"],
                    "power": move_data["power"],
                    "move_type": move_data["damage_class"]["name"],
                    "description": description
                }

# Save JSON
with open("gen2_crystal_all_moves.json", "w") as f:
    json.dump(moves_dict, f, indent=2)

print("Saved all Gen 2 Crystal moves (level-up + TM/HM) to 'gen2_crystal_all_moves.json'")
