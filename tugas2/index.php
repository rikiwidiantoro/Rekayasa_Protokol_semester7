<?php
	require ("koneksi.php"); 


?>

<!DOCTYPE html>
<html>
	<!-- <head>
	<meta http-equiv="refresh" content="5">
	
</head> -->
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
	<body>
		<style> 
		#wntable {
			border-collapse: collapse;
			width: 50%;
		}
		
		#wntable td, #wntable th {
			border: 1px solid #ddd;
			padding: 8px;
		}
		
		#wntable tr:nth-child (even) {background-color: #f2f2f2;}
		
		#wntable tr:hover {background-color: #ddd;}
		
		#wntable th {
			padding-top: 12 px;
			padding-bottom: 12px;
			text-align: left;
			background-color: #00A8A9;
			color: white;
		}
		</style>


		<div id="cards" class="cards" align="center" >
			<h1> Data Sensor Kelembapan Warriornux </h1>
			<table id="wntable">
			<tr>
				<th>No</th>
				<th>Data</th>
				<th>Waktu</th>
			</tr>
			<?php
			
			$sql = mysqli_query($koneksi, "SELECT * FROM dht11 ORDER BY id DESC");
			
			if(mysqli_num_rows($sql) == 0) {
				echo '<tr><td colspan ="14">Data Tidak Ada. </td></tr>';
			}else {
				$no = 1;
				while($row = mysqli_fetch_assoc($sql)) {
					echo '
					<tr>
					<td>'.$no.'</td>
					<td>'.$row['humidity'].'</td>
					<td>'.$row['temperature']. '</td>
					</tr>
					';
					$no++; 
				}
				
			}
			?>
			</table>
		</div>
</body>
</html>
