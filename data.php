<?php
$conn = mysqli_connect('localhost', 'root', '', 'sensors');
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

if(isset($_POST['voltage']) && isset($_POST['current'])){
    $valueToStore_1 = $_POST['voltage'];
    $valueToStore_2 = $_POST['current'];
    $sql = "INSERT INTO `sensordatanew` (`Voltage_sensor`, `Current_sensor`) VALUES ('$valueToStore_1', '$valueToStore_2')";
    $request = mysqli_query($conn, $sql);

    if ($request) {
        echo "Value stored successfully!";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$sql = "SELECT `Voltage_sensor`, `Current_sensor` FROM `sensordatanew` ORDER BY `id` DESC LIMIT 1";
$result = mysqli_query($conn, $sql);
$latestData = ['Voltage_sensor' => 'No data', 'Current_sensor' => 'No data'];

if ($result && mysqli_num_rows($result) > 0) {
    $latestData = mysqli_fetch_assoc($result);
}

$conn->close();

$power = $latestData['Voltage_sensor'] * 5;
?>

<!DOCTYPE html>
<html>
<head>
    <title>Sensor Data</title>
    <meta http-equiv="refresh" content="1">
    <link rel="stylesheet" href="data.css">
    <script>
        function checkPower() {
            var power = <?php echo $power; ?>;
            if (power > 75) {
                var audio = document.getElementById('alertAudio');
                audio.play().catch(function(error) {
                    console.log("Autoplay prevented. User interaction is needed to play the audio.");
                });
            }
        }
    </script>
</head>
<body onload="checkPower()">
    <div class="SensorDataContainer">
        <h1 class="SensorDataHeading">Bulb Status</h1>
        <p class="SensorData">Voltage: <span id="voltage"><?php echo $latestData['Voltage_sensor']; ?></span></p>
        <p class="SensorData">Current: <span id="current"><?php echo $latestData['Current_sensor']; ?></span></p>
        <p class="SensorData">Power: <span id="power"><?php echo $power; ?></span></p>
    </div>
    <audio id="alertAudio" src="./alert.mp3"></audio>
</body>
</html>
