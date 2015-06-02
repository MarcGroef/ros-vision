/**
 * Created by admin on 13-3-14.
 */
$(function() {
    $('.foto-album-clickable').each(function(){
        $(this).off().on('click', function(){
            url = $(this).data('url');
            changePicture(url);
            $(".foto-album-overlay").slideDown( 600, 'swing', function(){
                height = $(".image-overlay img").height()/2;
                $(".arrow-container").css('top',height+100);
            });

            setNextButton(url);
            setPreviousButton(url);
        });
    });
    $(".image-overlay").off().on('click', function(){
        $(".foto-album-overlay").hide();
    });

});

$(window).resize(function() {
    height = $(".image-overlay img").height()/2;
    $(".arrow-container").css('top',height+100);
});
function changePicture(url){
    $(".foto-album-overlay img").attr('src', url);
    setNextButton(url);
    setPreviousButton(url);
}

function setNextButton(url){
    next = $('li a[data-url="' + url + '"]').parent().next().children().data('url');
    if(next !== undefined){
        $(".foto-album-arrow-right").show().off().on('click', function(url){
            changePicture(next);
        });
    } else {
        $(".foto-album-arrow-right").hide();
    }
}

function setPreviousButton(url){
    prev = $('li a[data-url="' + url + '"]').parent().prev().children().data('url');
    if(prev !== undefined){
        $(".foto-album-arrow-left").show().off().on('click', function(url){
            changePicture(prev);
        });
    } else {
        $(".foto-album-arrow-left").hide();
    }
}