<?php
if (isset($_GET['format'])) {
    $format = $_GET['format'];

    if ($format === 'csv') {
        header('Content-Type: text/csv');
        header('Content-Disposition: attachment; filename="infos_thingspeak.csv"');

        // récupérer les données du ThingSpeak au format CSV
        $channelId = 2535771; // ID canal ThingSpeak
        $apiKey = 'G1SLZUJLOQ0QOBX0'; // Clé API ThingSpeak
        $url = "https://api.thingspeak.com/channels/{$channelId}/feeds.csv?api_key={$apiKey}&timezone=UTC&results=60";

        // récupérer le contenu CSV depuis ThingSpeak
        $csvContent = file_get_contents($url);

        // envoyer réponse avec le contenu CSV
        echo $csvContent;
        exit;
    } elseif ($format === 'sql') {
        header('Content-Type: text/sql');
        header('Content-Disposition: attachment; filename="alls_fields_data.sql"');
    
        // récupérer les données du ThingSpeak au format CSV
        $channelId = 2535771; // ID canal ThingSpeak
        $apiKey = 'G1SLZUJLOQ0QOBX0'; // Clé API ThingSpeak
        $url = "https://api.thingspeak.com/channels/{$channelId}/feeds.csv?api_key={$apiKey}&timezone=UTC&results=60";
    
        // récupérer les données CSV depuis ThingSpeak
        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
        $csvData = curl_exec($curl);
        curl_close($curl);
    
        // vérifiecation des données 
        echo "Contenu CSV : <br>";
        echo nl2br($csvData); 
        echo "<br><br>";
    
        // convertir CSV en tableau de lignes
        $lines = explode("\n", $csvData);
    
        // Initialiser un tableau pour stocker les commandes SQL
        $sqlCommands = [];
    
        // Parcourir chaque ligne de données CSV
        foreach ($lines as $line) {
            // Ignorer la première ligne (en-tête)
            if (!empty($line)) {
                $fields = str_getcsv($line);
    
                // Vérifier que la ligne a au moins 5 champs (timestamp + 4 valeurs)
                if (count($fields) >= 5) {
                    $timestamp = $fields[1]; // Timestamp est le deuxième champ
                    $values = array_slice($fields, 2, 4); // Récupérer les valeurs des champs 2 à 5
    
                    // Échapper les valeurs pour les inclure dans la commande SQL
                    $escapedValues = array_map(function($value) {
                        return "'" . addslashes($value) . "'";
                    }, $values);
    
                    // Vérifier si le timestamp est valide (non vide)
                    if (!empty($timestamp)) {
                        // Générer la commande SQL d'insertion avec les valeurs
                        $sqlCommand = "INSERT INTO data (timestamp, field1, field2, field3, field4) VALUES ('$timestamp', " . implode(', ', $escapedValues) . ");";
                        
                        // Ajouter la commande SQL au tableau
                        $sqlCommands[] = $sqlCommand;
                        
                        echo "Commande SQL générée : $sqlCommand <br>";
                    }
                }
            }
        }
    
        // affichage des commandes SQL générées 
        echo "<br> Commandes SQL générées : <br>";
        echo "<pre>";
        print_r($sqlCommands);
        echo "</pre>";
    
        // envoyer réponse avec commandes SQL 
        echo implode("\n", $sqlCommands);
        exit;
    }
}

// redirection en cas de probleme
header('Location: thinkspead_charts.php');
exit;
?>

