import shutil
import os

data_folder = "path to your folder containing all inverse cooking JSONs"
output_folder = "path to your output folder for extracted inverse cooking JSONs"
image_retrieval_extracted_path = "path to your txt file containing filtered image names for inverse image extracted by retrieval pairs"
os.makedirs(output_folder, exist_ok=True)

# Read image names from txt file
with open(image_retrieval_extracted_path, "r") as f:
    image_names = [
        line.strip().replace('"', "").replace(".jpg", "") for line in f if line.strip()
    ]

not_found_file = os.path.join(output_folder, "not_found_images.txt")
batch_size = 571

with open(not_found_file, "w") as nf:
    for idx, image_name in enumerate(image_names):
        start_idx = (idx // batch_size) * batch_size + 1
        end_idx = min((idx // batch_size + 1) * batch_size, len(image_names))
        batch_folder_name = f"Image{start_idx}to{end_idx}"
        batch_folder = os.path.join(output_folder, batch_folder_name)
        os.makedirs(batch_folder, exist_ok=True)
        print(f"Processing {image_name} into {batch_folder}...")
        found = 0
        for root, dirs, files in os.walk(data_folder):
            for file in files:
                if file.startswith(f"Image{image_name}") and file.endswith(".json"):
                    src_path = os.path.join(root, file)
                    dst_path = os.path.join(batch_folder, file)
                    shutil.copy2(src_path, dst_path)
                    found += 1
            if found >= 2:
                break  # Stop searching once both JSONs are found for this image_name
        if found == 0:
            nf.write(image_name + "\n")
