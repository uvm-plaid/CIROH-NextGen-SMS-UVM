<?php
$audio_dir = 'uploads';
$categorized_dir = 'categorized_files';

// Define allowed categories
$allowed_categories = ['HeavyRain', 'MediumRain', 'LightRain', 'Snow', 'HeavyHail', 'LightHail', 'WhiteNoise', 'Delete'];

if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    if (isset($_GET['action'])) {
        $action = $_GET['action'];

        // List files in the audio directory
        if ($action === 'list_files') {
            header('Content-Type: application/json');
            $files = array_diff(scandir($audio_dir), array('.', '..'));
            error_log("Found files: " . print_r($files, true));
            die(json_encode(array_values($files)));
        }

        // Move file to the selected category
        elseif ($action === 'move_file' && isset($_GET['file'], $_GET['category'])) {
            $file = basename($_GET['file']); // Prevent directory traversal
            $category = $_GET['category'];
            
            // Validate category
            if (!in_array($category, $allowed_categories)) {
                http_response_code(400);
                echo "Invalid category";
                exit;
            }
            
            // Validate file exists and is audio
            $source = "$audio_dir/$file";
            if (!file_exists($source) || !is_file($source)) {
                http_response_code(404);
                echo "File not found";
                exit;
            }
            

            $destination = "$categorized_dir/$category/$file";

            if (file_exists($source)) {
                if (!is_dir("$categorized_dir/$category")) {
                    mkdir("$categorized_dir/$category", 0777, true);
                }
                if (rename($source, $destination)) {
                    echo "File '$file' moved to category '$category'.";
                } else {
                    echo "Failed to move file '$file'.";
                }
            } else {
                echo "File '$file' does not exist.";
            }
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Weather Categorizer</title>
    <style>
        body {
            font-family: Arial, sans-serif;
        }
        .audio-container {
            margin-bottom: 20px;
        }
        select, button {
            margin-left: 10px;
        }
    </style>
</head>
<body>
    <h1>Weather Categorizer</h1>
    <div id="audioList">
        <!-- Audio files will be dynamically added here -->
    </div>

    <script>
        // Fetch the list of audio files from the server
        fetch('index.php?action=list_files')
            .then(response => {
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                return response.json();
            })
            .then(files => {
                console.log('Files received:', files);
                const audioList = document.getElementById('audioList');
                files.forEach(file => {
                    // Create an audio player, dropdown, and button for each file
                    const container = document.createElement('div');
                    container.className = 'audio-container';

                    const audio = document.createElement('audio');
                    audio.src = `uploads/${file}`;
                    audio.controls = true;

                    const select = document.createElement('select');
                    ['Heavy Rain', 'Medium Rain', 'Light Rain', 'Snow', 'Heavy Hail', 'Light Hail', 'WhiteNoise', 'Delete'].forEach(optionText => {
                        const option = document.createElement('option');
                        option.value = optionText.replace(' ', '');
                        option.textContent = optionText;
                        select.appendChild(option);
                    });

                    const button = document.createElement('button');
                    button.textContent = 'Categorize';
                    button.addEventListener('click', () => {
                        const category = select.value;
                        fetch(`index.php?action=move_file&file=${file}&category=${category}`)
                            .then(response => response.text())
                            .then(message => {
                                alert(message);
                                container.remove(); // Remove the categorized file from the list
                            });
                    });

                    container.appendChild(audio);
                    container.appendChild(select);
                    container.appendChild(button);
                    audioList.appendChild(container);
                });
            })
            .catch(error => {
                console.error('Error:', error);
                document.getElementById('audioList').innerHTML = 'Error loading files: ' + error.message;
            });
    </script>
</body>
</html>