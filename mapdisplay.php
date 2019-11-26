<?php


$servername = "localhost";
$username = "*";
$password = "*";
$dbname = "*";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

//$sql = "SELECT * FROM `gpsdata` WHERE userIP =1";


$sql = "SELECT * FROM `gpsdata` WHERE userIP = 1 ORDER BY ID Desc LIMIT 1";


$result = $conn->query($sql);

if ($result->num_rows > 0) {  ?>

<!DOCTYPE html>
<html>
  <head>
    <style>

      body {background-color: #ffcc18;}

       /* Set the size of the div element that contains the map */
      #map {
        height: 600px;  /* The height is 400 pixels */
        width: 100%;  /* The width is the width of the web page */
       }
.blinking{
	animation:blinkingText 1s infinite;
}
@keyframes blinkingText{
	0%{		color: red;	}
	49%{	color: transparent;	}
	50%{	color: transparent;	}
	99%{	color: transparent;	}
	100%{	color: #000;	}
}
.footer {
  position: fixed;
  left: 0;
  bottom: 0;
  width: 100%;
  background-color: black;
  color: white;
  text-align: center;
}
    </style>
<div class="footer">
  <p>iStudio Technologies </p>
</div
  </head>
  <body>
<div class="header" style="background-color:black;color:white;text-align: center;
padding:3px;">

    <h2>Tracker</h2>
</div> 
    <!--The div element for the map -->
    <div id="map"></div>

<?php   while($row = $result->fetch_assoc()) {



echo " <p style='border: 2.5px solid black';>". " Latitude : " ."<span style ='color:blue';>".$row['latitude']."</span>"."<br>";

echo " Longitude : " ."<span style ='color:blue';>".$row['longitude']."</span>"."<br>";

echo " User IP : "."<span style ='color:blue';>". $row['userIP']."</span>"."<span class='blinking'>". " Panic "."</span>"."<br>";

echo " Date and Time : "."<span style ='color:blue';>".$row['date']."</span>"."<span class='blinking'>". " ( Latest update ) "."</span>"."<br>";


 ?>





    <script>
// Initialize and add the map
function initMap() {
  // The location of Uluru
  var uluru = {lat: <?=$row['latitude']?>, lng: <?=$row['longitude']?>};
  // The map, centered at Uluru
  var map = new google.maps.Map(
      document.getElementById('map'), {zoom: 4, center: uluru});
  // The marker, positioned at Uluru
  var marker = new google.maps.Marker({position: uluru, map: map});
}
    </script>

<?php    }  ?>
    <!--Load the API from the specified URL
    * The async attribute allows the browser to render the page while the API loads
    * The key parameter will contain your own API key (which is not needed for this tutorial)
    * The callback parameter executes the initMap() function
    -->
    <script async defer
    src="https://maps.googleapis.com/maps/api/js?key=yourapi&callback=initMap">
    </script>
  </body>
</html>

<?php } else {
    echo "0 results";
}
$conn->close();

?>
