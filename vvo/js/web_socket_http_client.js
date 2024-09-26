console.log('vvoStream client: initialized script');

let ws = new WebSocket('ws://' + window.location.host + '/vvoStreamWS');
ws.binaryType = 'arraybuffer';  // Ensure binary data is received as an ArrayBuffer


ws.onmessage = function(event) {
    
    console.log('vvoStream client: received message from server: ', event.data);
    
    const imgElement = document.getElementById('image');
    
    if (imgElement.src) {//release old image
      URL.revokeObjectURL(imgElement.src);
    }

    const arrayBuffer = event.data;
    const blob        = new Blob([arrayBuffer], { type: 'image/png' });
    imgElement.src    = URL.createObjectURL(blob);
};

ws.onopen = function() {
  // Connection established\
};