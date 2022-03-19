// add n cubes to the html page
function addCubes(n) {
  var html = "";
  for (var i = 0; i < n; i++) {
    html +=
      '<div class="cube" id="cube_' +
      (i + 1) +
      '"><div class="square" id="square1"></div><div class="square" id="square2"></div><div class="square" id="square3"></div><div class="square" id="square4"></div><div class="square" id="square5"></div><div class="square" id="square6"></div><div class="square" id="square7"></div><div class="square" id="square8"></div><div class="square" id="square9"></div></div>';
  }
  $("#grid").html(html);
}

function getRandomColor() {
  var letters = "0123456789ABCDEF";
  var color = "#";
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
}

function randomColors() {
  // for each cube in the grid
  $(".cube").each(function () {
    // for each square in the cube
    $(this)
      .children()
      .each(function () {
        // change the color of the square
        $(this).css("background-color", getRandomColor());
      });
  });
}

function setSize() {
  // get the size from the input
  var width = $("#width").val();
  var height = $("#height").val();

  // set the size of the grid
  $("#grid").css("grid-template-columns", "repeat(" + width + ", 0fr)");
  $("#grid").css("grid-template-rows", "repeat(" + height + ", 0fr)");

  // add the cubes
  addCubes(width * height);

  $("#grid").css("width", width * 65 + "px");
  $("#grid").css("height", height * 65 + "px");

  $("#grid").css("background-color", "gray");

  // unhide the download button
  if (width > 0 && height > 0) {
    $("#download").css("display", "block");
  }
}

function getColorsFromImage() {
  // get pixel data from image file in /images
  var img = new Image();
  img.src = "images/output.jpg";
  img.onload = function () {
    var canvas = document.createElement("canvas");
    canvas.width = img.width;
    canvas.height = img.height;
    var ctx = canvas.getContext("2d");
    ctx.drawImage(img, 0, 0);
    var imgData = ctx.getImageData(0, 0, img.width, img.height);
    var data = imgData.data;
    // console.log(data);
    var colors = [];
    for (var i = 0; i < data.length; i += 4) {
      var r = data[i];
      var g = data[i + 1];
      var b = data[i + 2];
      var color = "rgb(" + r + "," + g + "," + b + ")";
      colors.push(color);
    }

    // console.log(colors.length);
    // console.log(colors);

    // set the colors of the cubes
    // $(".cube").each(function (i) {
    //   $(this)
    //     .children()
    //     .each(function (j) {
    //       $(this).css("background-color", colors[i * 9 + j]);
    //     });
    // });

    // for each square set the color
    $(".square").each(function (i) {
      $(this).css("background-color", colors[i]);
    });
  };
}

function downloadImage() {
  // get the image
  var canvas = document.getElementById("grid");
  html2canvas(canvas).then(function (canvas) {
    var img = canvas.toDataURL("image/png");
    var link = document.createElement("a");
    link.download = $("#filename").val() + ".png";
    link.href = img;
    link.click();
  });
}
