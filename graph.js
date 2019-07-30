var hasBeenClicked=false;
$(function(){ // on dom ready
var ts;
$('#cy').cytoscape({
  style: cytoscape.stylesheet()
    .selector('node')
      .css({
        'content': 'data(name)',
        'text-valign': 'center',
        'font-size': 35,
        'font-weight': 'data(fontWeight)',
        'color': 'data(color)',
        'shape': 'roundrectangle',
        'width': 200,
        'height': 40,
        'background-color': 'data(backgroundColor)',
      })
    .selector('edge')
      .css({
        'target-arrow-shape': 'triangle',
        'width':6,
        'opacity': 0,
      })
    .selector(':selected')
      .css({
        'background-color': 'black',
        'target-arrow-color': 'black',
        'source-arrow-color': 'black',
      })
    .selector('node.faded')
      .css({
        'opacity': 'data(fadedOpacity)'

      })
    .selector('edge.unfaded')
      .css({
       'opacity': 1,
        'text-opacity': 1,
      })
    .selector('edge.faded')
      .css({
       'opacity': 0,
        'text-opacity': 0,
      })
      ,
      elements: elementData,

  layout: {
    name: 'preset',

  positions: undefined, // map of (node id) => (position obj); or function(node){ return somPos; }
  zoom: undefined, // the zoom level to set (prob want fit = false if set)
  pan: undefined, // the pan level to set (prob want fit = false if set)
  fit: true, // whether to fit to viewport
  padding: 30, // padding on fit
  animate: false, // whether to transition the node positions
  animationDuration: 500, // duration of animation in ms if enabled
  ready: undefined, // callback on layoutready
  stop: undefined, // callback on layoutstop
  }
    ,
  hideLabelsOnViewport: false,

  // on graph initial layout done (could be async depending on layout...)
  ready: function(){
    window.cy = this;
    cy.panningEnabled(false);
    cy.userPanningEnabled(true);
    cy.zoomingEnabled(false);
    cy.userZoomingEnabled(true);
    // giddy up...
    cy.elements().unselectify();
    cy.elements().lock();
    cy.on('tap', 'node', function(e){
      var node = e.cyTarget;
      if(hasBeenClicked)
      {
      		//alert(hasBeenClicked);
       cy.elements().removeClass('faded');
        cy.elements().removeClass('unfaded');
      		hasBeenClicked=false;
      }
      else
      {
      var neighborhood = node.neighborhood().add(node);

      cy.elements().addClass('faded');
      neighborhood.removeClass('faded');
      neighborhood.addClass('unfaded');
      hasBeenClicked=true;
      }
    });
    cy.on('tap', function(e){

      if( e.cyTarget === cy ){
        cy.elements().removeClass('faded');
        cy.elements().removeClass('unfaded');
      }
    });

  }
});

}); // on dom ready
