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

    // Create unique filename
    $timestamp = date("Ymd_His"); // Format: YYYYMMDD_HHMMSS
    $targetFile = $targetDir . "audio" . $timestamp . ".WAV";

    // Move the uploaded file to the target directory
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo json_encode(["success" => "File uploaded successfully.", "filename" => basename($targetFile)]);
    } else {
        echo json_encode(["error" => "There was an error uploading the file."]);
    }
} else {
    echo json_encode(["error" => "No file was uploaded or an error occurred during upload."]);
}
?>
