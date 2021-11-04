<!DOCTYPE HTML>
<!--
	Escape Velocity by HTML5 UP
	html5up.net | @ajlkn
	Free for personal and commercial use under the CCA 3.0 license (html5up.net/license)
-->
<html>
	<head>
		<title>PIA12 FISH TANK - AQUARIUM</title>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
		<link rel="stylesheet" href="sito/assets/css/main8.css" />
    <?php
      include("connection.php");
      if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
      }
      $sqlT = "SELECT temperature, data_send as send FROM my_myfishtank.temp_tab ORDER BY data_send DESC limit 1";
      $sqlEC = "SELECT ec, data_send as send FROM my_myfishtank.ec_tab ORDER BY data_send DESC limit 1";
      $sqlTDS = "SELECT tds, data_send as send FROM my_myfishtank.tds_tab ORDER BY data_send DESC limit 1";
          
      $resultT = $conn->query($sqlT);
      $resultEC = $conn->query($sqlEC);
      $resultTDS = $conn->query($sqlTDS);
     
      if ($resultT->num_rows > 0) {
        // output data of each row
        while($rowT = $resultT->fetch_assoc()) {
          $temperature = $rowT["temperature"];
          $sendT = $rowT["send"];
        }
      } else {
        $temperature = 0;
        $sendT = "no data";
      }
      
       if ($resultEC->num_rows > 0) {
        // output data of each row
        while($rowEC = $resultEC->fetch_assoc()) {
          $ec = $rowEC["ec"];
          $sendEC = $rowEC["send"];
        }
      } else {
        $ec = 0;
        $sendEC = "no data";
      }
      
       if ($resultTDS->num_rows > 0) {
        // output data of each row
        while($rowTDS = $resultTDS -> fetch_assoc()) {
          $tds = $rowTDS["tds"];
          $sendTDS = $rowTDS["send"];
        }
      } else {
        $tds = 0;
        $sendTDS = "no data";
      }
      
      $conn->close();
    ?>
  </head>
  <body class="homepage is-preload">
    <div id="page-wrapper">
      <!-- Header -->
      <section id="header" class="wrapper">
        <!-- Logo -->
        <div id="logo">
            <h1><a href="index.php">AQUARIUM PIA12</a></h1>
            <p>The easy way to manage your fish tank</p>
        </div>
        <!-- Nav -->
        <!--<nav id="nav">
          <ul>
            <li class="current"><a href="index.html">Home</a></li>
            <li>
              <a href="#">Measurements</a>
                <ul>
                  <li><a href="temperature.php">Temperature</a></li>
                  <li><a href="ph.txt">PH</a></li>
                  <li><a href="ec.php">EC</a></li>
                  <li><a href="tds.php">TDS</a></li>
                </ul>
              </li>
            <li><a href="left-sidebar.html">Left Sidebar</a></li>
            <li><a href="right-sidebar.html">Right Sidebar</a></li>
            <li><a href="no-sidebar.html">Contatti</a></li>
          </ul>
        </nav>-->
      </section>

      <!-- Intro -->
      <section id="intro" class="wrapper-carousel style1">
        <div class="container">
          <div class="carousel" data-flickity='{ "wrapAround": true, "autoPlay": true, "imagesLoaded":true }'>
              <div class="carousel-cell">
                  <img class="w3-image" src="images\aquarium.jpg">
              </div>
              <div class="carousel-cell">
                  <img class="w3-image" src="images\aquarium1.jpg">
              </div>
              <div class="carousel-cell">
                  <img class="w3-image" src="images\aquarium2.jpg">
              </div>
              <div class="carousel-cell">
                  <img class="w3-image" src="images\aquarium3.jpg">
              </div>
              <div class="carousel-cell">
                  <img class="w3-image" src="images\aquarium4.jpg">
              </div>
          </div>
        </div>
      </section>
      <!-- DashBoard -->        
      <section id="highlights" class="wrapper style3">
        <div class="title">DashBoard</div>
        <div class="container">
          <div class="row aln-center">
            <div class="col-4 col-12-medium">
              <section class="highlight">
                <h3>Temperature</h3>
                <p class="style4"> Recorded on <?php echo gmdate("l jS \of F Y", $sendT). " at ".gmdate("H:i:s", $sendT);?></p>
                <div class="container_chart">
                  <div class="row_chart">        
			        <div id="chart_temp"></div>
                  </div>
                </div>    
                <ul class="actions">
                  <li><a href="temperature.php" class="button style3">More Details</a></li>
                </ul>
			  </section>
			</div>
			<div class="col-4 col-12-medium">
			  <section class="highlight">
                <h3>Conductivity</h3>
                <p class="style4"> Recorded on <?php echo gmdate("l jS \of F Y", $sendEC). " at ".gmdate("H:i:s", $sendEC);?></p>
			    <div class="container_chart">
                  <div class="row_chart">
                    <div id="chart_ec"></div>
                  </div>
                </div>			
                <ul class="actions">
                  <li><a href="ec.php" class="button style3">More Details</a></li>
                </ul>
		      </section>
			</div>
			<div class="col-4 col-12-medium">
			  <section class="highlight">
			    <h3>Total Dissolved Solids</h3>
                <p class="style4"> Recorded on <?php echo gmdate("l jS \of F Y", $sendTDS). " at ".gmdate("H:i:s", $sendTDS);?></p>
                <div class="container_chart">
                  <div class="row_chart">
                    <div id="chart_tds"></div>
                  </div>
                </div>
                <ul class="actions">
                  <li><a href="tds.php" class="button style3">More Details</a></li>
                </ul>
		  	  </section>
            </div>
            <div class="col-4 col-12-medium">
			  <section class="highlight">
                <h3>PH</h3>
                <p class="style4"> Recorded on <?php echo $sendT;?></p>
			    <div class="container_chart">
                  <div class="row_chart">
                    <div id="chart_ph"></div>
                  </div>
                </div>
                <ul class="actions">
                  <li><a href="#" class="button style3">More Details</a></li>
                </ul>
			  </section>
            </div>
          </div>
        </div>
      </section>
      <!-- Main -->
<!--  <section id="main" class="wrapper style2">
        <div class="title">EC</div>
        <div class="container">

        </div>
      </section> -->
      <!-- Main -->
<!--  <section id="main" class="wrapper style4">
        <div class="title">PH</div>
        <div class="container">

        </div>
      </section> -->        
      <!-- Footer -->
      <section id="footer" class="wrapper">
        <div class="container">
          <div class="row">
            <div class="col-6 col-12-medium">
            <!-- Contact -->
              <section class="feature-list small">
                <div class="row">
                  <div class="col-6 col-12-small">
                    <section>
                      <h3 class="icon solid fa-comment">Social</h3>
                      <p>
                        <a href="https://github.com/PasqualeAuriemma/MyFishTankProject">Github</a><br />
                        <a href="https://www.linkedin.com/in/pasquale-auriemma-780953b8/">LinkedIn</a><br />
                        <a href="#">Untitled</a>
                      </p>
                    </section>
                  </div>
                  <div class="col-6 col-12-small">
                    <section>
                      <h3 class="icon solid fa-envelope">Email</h3>
                        <p>
                          <a href="#">info@untitled.tld</a>
                        </p>
                    </section>
                  </div>
                </div>
              </section>
            </div>
          </div>
          <div id="copyright">
            <a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Licenza Creative Commons" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a>
            <br />Quest'opera è distribuita con Licenza <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribuzione 4.0 Internazionale</a>.
            
          </div>
        </div>
      </section>
    </div>

    <!-- Scripts -->
    <script src="sito/assets/js/jquery.min.js"></script>
    <script src="sito/assets/js/jquery.dropotron.min.js"></script>
    <script src="sito/assets/js/browser.min.js"></script>
    <script src="sito/assets/js/breakpoints.min.js"></script>
    <script src="sito/assets/js/util.js"></script>
    <!-- <script src="sito/assets/js/main.js"></script> -->
    <!-- oppure <script src="https://unpkg.com/flickity@2/dist/flickity.pkgd.min.js"></script> -->
    <script src="sito/assets\js\flickity.pkgd.min.js"></script>
    <script type="text/javascript" src="https://www.google.com/jsapi"></script>
    <script type="text/javascript">
      google.load('visualization', '1', {packages: ['corechart', 'gauge']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {

        var dataTemp = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['Gradi °', <?php echo $temperature;?>],
        ]);
        
        var dataTDS = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['ppm', <?php echo $tds;?>],
        ]);

        var dataEC = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['µS/cm', <?php echo $ec;?>],
        ]);

        var dataPH = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['pH', 7],
        ]);
          
        var w = $(window).width();
        var x = Math.floor(w * 0.3);
        //console.log("width: " + w + ", x = " + x);
        var h = $(window).height();
        var y = Math.floor(h * 0.3);
        //console.log("height: " + h + ", y = " + y);

        var options = {
            yellowFrom: 28, yellowTo: 34,
            redFrom: 34, redTo: 45,
            minorTicks: 5,
            max: 45,
            height: y,
            width: x
        };

         var optionsTDS = {
            yellowFrom: 500, yellowTo: 650,
            redFrom: 650, redTo: 1000,
            minorTicks: 0,
            max: 1000,
            height: y,
            width: x
        }; 
        
        var optionsEC = {
            yellowFrom: 900, yellowTo: 920,
            redFrom: 920, redTo: 1000,
            minorTicks: 0,
            max: 1000,
            height: y,
            width: x
        }; 
        
        var optionsPH = {
            yellowFrom: 0, yellowTo: 6.5,
            greenFrom: 6.5, greenTo: 8,
            redFrom: 8, redTo: 12,
            minorTicks: 0,
            max: 12,
            height: y,
            width: x
        };
          
        function resize() {    
          var chartT = new google.visualization.Gauge(document.getElementById('chart_temp'));
          var chartE = new google.visualization.Gauge(document.getElementById('chart_ec'));
          var chartS = new google.visualization.Gauge(document.getElementById('chart_tds'));
          var chartP = new google.visualization.Gauge(document.getElementById('chart_ph'));
          chartT.clearChart();
          chartT.draw(dataTemp, options);
          chartE.clearChart();
          chartE.draw(dataEC, optionsEC);
          chartS.clearChart();
          chartS.draw(dataTDS, optionsTDS);
          chartP.clearChart();
          chartP.draw(dataPH, optionsPH);
        }
        window.onload = resize();
        window.onresize = resize;
      }
    </script>  
  </body>
</html>