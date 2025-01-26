<?php
$targetDir = "uploads/";

if (!is_dir($targetDir)) {
    mkdir($targetDir, 0777, true);
}

if (isset($_FILES['file']) && $_FILES['file']['error'] == UPLOAD_ERR_OK) {
    $fileType = strtolower(pathinfo($_FILES['file']['name'], PATHINFO_EXTENSION));
    if ($fileType != "wav") {
        echo json_encode(["error" => "Only .WAV files are allowed."]);
        exit;
    }

    $targetFile = $targetDir . basename($_FILES['file']['name']);

    if (file_exists($targetFile)) {
        echo json_encode(["error" => "A file with the same name already exists."]);
        exit;
    }

    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo json_encode(["success" => "File uploaded successfully.", "filename" => basename($targetFile)]);
    } else {
        echo json_encode(["error" => "There was an error uploading the file."]);
    }
} else {
    echo json_encode(["error" => "No file was uploaded or an error occurred during upload."]);
}
