<!DOCTYPE HTML>

<html>
  <head>
    <title>PIA12 FISH TANK - AQUARIUM</title>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no" />
    <link rel="stylesheet" href="assets/css/main14.css" />
  </head>
  <body class="homepage is-preload">
    <div id="page-wrapper">
      <!-- Header -->
      <section id="header-sub" class="wrapper">
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
      <!-- TDS -->
      <section id="tdsec" class="wrapper style5">
        <div class="title">Total Dissolved Solids and Conductivity</div>
        <div class="container">
          <div class="row aln-center">
            <div class="col-6 col-12-medium">
              <section class="highlight">
                 <p class="style2">
                  Overview with charts
              </p>
              </section>
            </div>    
            <div class="col-6 col-12-medium">
              <section class="highlight"> 
                <div style="width: 100%; height: 100%;">
                <h3><?php
                      $dataNow1 = date('d-m-Y');
                      echo "On $dataNow1 arrived:";
                    ?>
                </h3>  
                <?php
                  $dataNow1 = date('Y-m-d');  
                  $query4 = "SELECT t.data_send as date, t.tds, e.ec FROM my_myfishtank.tds_tab t JOIN my_myfishtank.ec_tab e ON t.id = e.id WHERE FROM_UNIXTIME(t.data_send, '%Y-%m-%d') = '$dataNow1' ORDER BY t.data_send";
                  
                  include("connection.php");
                  $result_list = $conn->query($query4);
                  $conn->close();
                  
                  echo "<ol>";
                  if ($result_list->num_rows > 0) {
                    while($row4 = $result_list->fetch_assoc()) {
                      echo "<li class='style1'> At ".gmdate("H:i:s\ ", $row4['date'])."  TDS = ".$row4['tds']." ppm - EC = ".number_format($row4['ec'], 1)." µS/cm</li>";
                    }
                  }
                  echo '</ol>';
                ?>
                </div>
              </section>
            </div>
          </div>         
          <div id="areachartTDS" style="margin: 0px 2px;"></div>
          <div id="buttonAreaChart">
            <button4 id="change7D" class="buttonTrend">7 Days</button4>
            <button1 id="change1M" class="buttonTrend">1 Month</button1>
            <button2 id="change2M" class="buttonTrend">2 Months</button2>
            <button3 id="changeALL" class="buttonTrend">All</button3>
          </div>
          <p style="clear:both"><br>
          <section class="highlight">
          <div id="areachartEC" style="margin: 0px 2px;"></div>
          <div id="buttonAreaChartEC">
            <button5 id="changeEC7D" class="buttonTrend">7 Days</button5>
            <button6 id="changeEC1M" class="buttonTrend">1 Month</button6>
            <button7 id="changeEC2M" class="buttonTrend">2 Months</button7>
            <button8 id="changeECALL" class="buttonTrend">All</button8>
          </div>
          </section>
          <p style="clear:both"><br>
          <section class="highlight">
            <div id="columnchart" style="width: 100%; height: 100%; overflow:auto; position: relative;"></div> 		
          </section>
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
      google.load('visualization', '1', {packages: ['corechart', 'bar', "calendar"]});
      google.setOnLoadCallback(setDataForAreaChart);

      function setDataForAreaChart(){

        <?php // PHP Google Charts
        $dataNow = date('Y-m-d', strtotime("-1 month"));
        $query2 = "SELECT FLOOR(tds) as tds, count(id) as count FROM my_myfishtank.tds_tab GROUP BY FLOOR(tds)";
        $query1 = "SELECT data_arrive as date, tds FROM my_myfishtank.tds_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send"; 
        $query3 = "SELECT FROM_UNIXTIME(data_send, '%Y,%m -1 ,%d') as data, COUNT(data_send) as value FROM my_myfishtank.tds_tab GROUP BY FROM_UNIXTIME(data_send, '%Y-%m-%d')"; 
        $query1EC = "SELECT data_arrive as date, ec FROM my_myfishtank.ec_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send"; 
        ?> 
        drawPieAreaColumnCharts();
      }

      var data_val;
      var data_val2;
      var data_val3;
      var data_valEC;
      function drawPieAreaColumnCharts() {
        data_val = google.visualization.arrayToDataTable([
          ['Date', 'TDS'],
          <?php // PHP Google Charts
          include("connection.php");
          $result1 = $conn->query($query1);

          if ($result1->num_rows > 0) {
            // output data of each row
            while($row1 = $result1->fetch_assoc()) {
              echo "['".$row1['date']."',".$row1['tds']."],";
            }
          }
          ?>
        ]);
        var options_val = {  
          height: '1600px',
          width: '80px',
          title: 'Total Dissolved Solids trend',
          hAxis: {textPosition: 'none'},
          explorer: { maxZoomIn: .5 , maxZoomOut: 8 },
          backgroundColor: '#f1f8e9',
          vAxis: {'title': 'TDS', 'minValue': 400, 'maxValue': 600},
          chartArea:{
            top: 36,
            left: 100,
            right: 18,
            bottom: 36
          },
        };
        
        data_valEC = google.visualization.arrayToDataTable([
          ['Date', 'EC'],
          <?php // PHP Google Charts
          include("connection.php");
          $result1 = $conn->query($query1EC);

          if ($result1->num_rows > 0) {
            // output data of each row
            while($row1 = $result1->fetch_assoc()) {
              echo "['".$row1['date']."',".$row1['ec']."],";
            }
          }
          ?>
        ]);
        
        var options_valEC = {  
          height: '1400px',
          width: '100px',
          title: 'Conductivity trend',
          hAxis: {textPosition: 'none'},
          explorer: { maxZoomIn: .5 , maxZoomOut: 8 },
          backgroundColor: '#f1f8e9',
          vAxis: {'title': 'Temperature', 'minValue': 680, 'maxValue': 850},
          chartArea:{
            top: 36,
            left: 100,
            right: 18,
            bottom: 36
          },
        };
        
        var dataTable = new google.visualization.DataTable();
        dataTable.addColumn({ type: 'date', id: 'Date' });
        dataTable.addColumn({ type: 'number', id: 'Registrations' });
        dataTable.addRows([
          <?php // PHP Google Charts
          $result3 = $conn->query($query3);
          if ($result3->num_rows > 0) {
            // output data of each row
            while($row3 = $result3->fetch_assoc()) {
              echo "[new Date(".$row3['data']."),".$row3['value']."],";
            }
          }
        ?>]);

        var options_val3 = {
          width: '950',
          title: "Number of daily measurements",

          colorAxis: {minValue: 0,  colors: ['#FF0000', '#0903ab']},
          calendar: {
            yearLabel: {
              fontName: 'Times-Roman',
              fontSize: 32,
              color: '#21b8ce',
              bold: true,
              italic: true
            },
            dayOfWeekLabel: {
              fontName: 'Times-Roman',
              fontSize: 12,
              color: '#2f21ce',
              bold: false,
              italic: false
            }, 
            monthLabel: {
              fontName: 'Times-Roman',
              fontSize: 12,
              color: '#2f21ce',
              bold: true,
              italic: true
            },
            underMonthSpace: 16,
            daysOfWeek: 'SMTWTFS',
          }
        }

        data_val2 = google.visualization.arrayToDataTable([
          ['TDS', 'Count'],
          <?php 
          $result2 = $conn->query($query2);
          if ($result2->num_rows > 0) {
            // output data of each row
            while($row2 = $result2->fetch_assoc()) {
              echo "['".intval($row2['tds'], 10)."',".$row2['count']."],";
            }
          }
          ?>
        ]);
        
        var options_val2 = {
          pieSliceText: 'percentage',
          backgroundColor: 'transparent',
          is3D: true,
          chartArea:{left:'20%',top:0,width:'100%',height:'100%'},
          legend: {textStyle: {color: 'black', fontSize: 14}}
        };
        var chart_val3 = new google.visualization.Calendar(document.getElementById('columnchart'));
        var chart_val = new google.visualization.AreaChart(document.getElementById('areachartTDS'));
        var chart_valEC = new google.visualization.AreaChart(document.getElementById('areachartEC'));
        
        function resize() {  
          chart_val3.clearChart();
          chart_val3.draw(dataTable, options_val3);
          chart_val.clearChart();
          chart_val.draw(data_val, options_val);
          chart_valEC.clearChart();
          chart_valEC.draw(data_valEC, options_valEC);
        }

        window.onload = resize();
        window.onresize = resize;

        var button7D = document.getElementById('change7D');
        var button1M = document.getElementById('change1M');
        var button2M = document.getElementById('change2M');
        var buttonALL = document.getElementById('changeALL');
        var buttonEC7D = document.getElementById('changeEC7D');
        var buttonEC1M = document.getElementById('changeEC1M');
        var buttonEC2M = document.getElementById('changeEC2M');
        var buttonECALL = document.getElementById('changeECALL');

        buttonEC7D.onclick = function () {

          data_valEC = google.visualization.arrayToDataTable([
            ['Date', 'Temp'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-7 day"));
            $query31 = "SELECT data_arrive as date, ec FROM my_myfishtank.ec_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            
            $result31 = $conn->query($query31);
            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['ec']."],";
              }
            }?>]);

          chart_valEC.clearChart(); 
          chart_valEC.draw(data_valEC, options_valEC);

        };
        
        buttonEC2M.onclick = function () {

          data_valEC = google.visualization.arrayToDataTable([
            ['Date', 'Temp'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-2 month"));
            $query31 = "SELECT data_arrive as date, ec FROM my_myfishtank.ec_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            
            $result31 = $conn->query($query31);
            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['ec']."],";
              }
            }?>]);

          chart_valEC.clearChart(); 
          chart_valEC.draw(data_valEC, options_valEC);

        };
        
        buttonEC1M.onclick = function () {

          data_valEC = google.visualization.arrayToDataTable([
            ['Date', 'EC'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-1 month"));
            $query31 = "SELECT data_arrive as date, ec FROM my_myfishtank.ec_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            $result31 = $conn->query($query31);

            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['ec']."],";
              }
            }
            ?>
          ]);
          chart_valEC.clearChart(); 
          chart_valEC.draw(data_valEC, options_valEC);

        };
        
        button7D.onclick = function () {

          data_val = google.visualization.arrayToDataTable([
            ['Date', 'TDS'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-7 day"));
            $query31 = "SELECT data_arrive as date, tds FROM my_myfishtank.tds_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            
            $result31 = $conn->query($query31);
            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['tds']."],";
              }
            }?>]);

          chart_val.clearChart(); 
          chart_val.draw(data_val, options_val);

        };
        
        buttonECALL.onclick = function () {

          data_valEC = google.visualization.arrayToDataTable([
            ['Date', 'EC'],
            <?php // PHP Google Charts

            $query31 = "SELECT data_arrive as date, ec FROM my_myfishtank.ec_tab ORDER BY data_send";          
            $result31 = $conn->query($query31);

            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['ec']."],";
              }
            }
            ?>
          ]);
          chart_valEC.clearChart(); 
          chart_valEC.draw(data_valEC, options_valEC);

        };
        
        button2M.onclick = function () {

          data_val = google.visualization.arrayToDataTable([
            ['Date', 'TDS'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-2 month"));
            $query31 = "SELECT data_arrive as date, tds FROM my_myfishtank.tds_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            
            $result31 = $conn->query($query31);
            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['tds']."],";
              }
            }?>]);

          chart_val.clearChart(); 
          chart_val.draw(data_val, options_val);

        };

        button1M.onclick = function () {

          data_val = google.visualization.arrayToDataTable([
            ['Date', 'TDS'],
            <?php // PHP Google Charts
            $dataNow = date('Y-m-d', strtotime("-1 month"));
            $query31 = "SELECT data_arrive as date, tds FROM my_myfishtank.tds_tab WHERE FROM_UNIXTIME(data_send, '%Y-%m-%d') >= '$dataNow' ORDER BY data_send";          
            $result31 = $conn->query($query31);

            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['tds']."],";
              }
            }
            ?>
          ]);
          chart_val.clearChart(); 
          chart_val.draw(data_val, options_val);

        };

        buttonALL.onclick = function () {

          data_val = google.visualization.arrayToDataTable([
            ['Date', 'TDS'],
            <?php // PHP Google Charts

            $query31 = "SELECT data_arrive as date, tds FROM my_myfishtank.tds_tab ORDER BY data_send";          
            $result31 = $conn->query($query31);

            if ($result31->num_rows > 0) {
              // output data of each row
              while($row31 = $result31->fetch_assoc()) {
                echo "['".$row31['date']."',".$row31['tds']."],";
              }
            }
            ?>
          ]);
          chart_val.clearChart(); 
          chart_val.draw(data_val, options_val);

        };
      }

   
      
      
    </script>
    <?php      
    $conn->close();
    ?>
  </body>
</html>