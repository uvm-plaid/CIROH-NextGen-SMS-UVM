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

    // Extract weather code from file name
    $fileName = basename($_FILES['file']['name']);
    preg_match('/\{(rain|snow|hail|drizzle|none)\}/', $fileName, $matches);

    if (!isset($matches[1])) {
        echo json_encode(["error" => "Invalid file name structure: missing weather code."]);
        exit;
    }

    $weatherCode = $matches[1];

    // Create subdirectory for the weather code if it doesn't exist
    $weatherDir = $targetDir . $weatherCode . "/";
    if (!is_dir($weatherDir)) {
        mkdir($weatherDir, 0777, true);
    }

    // Target file path in the weather-specific subdirectory
    $targetFile = $weatherDir . $fileName;

    // Check if the file already exists
    if (file_exists($targetFile)) {
        echo json_encode(["error" => "A file with the same name already exists in the directory."]);
        exit;
    }

    // Move the uploaded file to the target directory
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo json_encode(["success" => "File uploaded successfully.", "filename" => $fileName, "directory" => $weatherCode]);
    } else {
        echo json_encode(["error" => "There was an error uploading the file."]);
    }
} else {
    echo json_encode(["error" => "No file was uploaded or an error occurred during upload."]);
}
