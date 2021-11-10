<?php
//variabel database
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname ="dht";

    $koneksi = mysqli_connect($servername, $username, $password, $dbname);

    if(mysqli_connect_error()){ // mengecek apakah koneksi error
    echo 'Gagal melakukan koneksi ke Database : '.mysqli_connect_error();
    }
    ?>