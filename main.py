import zipfile
import os


# def zip_folder(folder_path, zip_path):
#     with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as zipf:
#         for root, dirs, files in os.walk(folder_path):
#             for file in files:
#                 abs_path = os.path.join(root, file)
#                 rel_path = os.path.relpath(abs_path, folder_path)
#                 zipf.write(abs_path, rel_path)


# root_dir = r"F:\\Recipe1M\\filtered_test"

# for subfolder in os.listdir(root_dir):
#     subfolder_path = os.path.join(root_dir, subfolder)
#     if os.path.isdir(subfolder_path):
#         zip_path = subfolder_path + ".zip"
#         zip_folder(subfolder_path, zip_path)
#         print(f"Zipped {subfolder_path} to {zip_path}")

# import pickle

# with open("det_ingrs.pkl", "rb") as f:
#     data = pickle.load(f)

# print(data)

# idx_list = []
# with open("F:/Revamping/output/not_found_pairs.txt", "r") as f:
#     for line in f:
#         idx = line.strip().split("\t")[0]
#         idx_list.append(idx)

# with open("not_found_idxs.txt", "w") as f:
#     for idx in idx_list:
#         f.write(idx + "\n")
# print(idx_list)

# not_found_pairs_indices = set()
# with open("not_found_idxs.txt", "r") as f:
#     for line in f:
#         idx = line.strip()
#         not_found_pairs_indices.add(idx)
# print(not_found_pairs_indices)

# sorted_indices = []
# with open("sorted_ids.txt", "r") as f:
#     for line in f:
#         idx = line.strip()
#         sorted_indices.append(idx)

# indices_to_remove = sorted(
#     [int(idx) - 1 for idx in not_found_pairs_indices if idx.isdigit()], reverse=True
# )

# for idx in indices_to_remove:
#     if 0 <= idx < len(sorted_indices):
#         del sorted_indices[idx]

# with open("filtered_sorted_idx.txt", "w") as f:
#     for idx in sorted_indices:
#         f.write(idx + "\n")

import shutil
import os

data_folder = r"F:/InverseCooking/"  # Change to your actual data folder
output_folder = (
    r"F:/InverseCookingTemp/filtered_jsons/"  # Change to your desired output folder
)
os.makedirs(output_folder, exist_ok=True)

# Read image names from txt file
with open("filtered_image_names_inverse.txt", "r") as f:
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
