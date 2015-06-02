$(function() {

    //browsercheck
    if ($.browser.msie && $.browser.version < 9) {
        var updateText = "Let op! Uw browser (Internet Explorer " + $.browser.version + ") is verouderd en heeft bekende beveiligingsproblemen. Deze website werkt daardoor mogelijk niet correct! Installeer de meest recente versie (Internet Explorer 10) of gebruik een alternatieve browser, zoals Chrome of Firefox.";
        var warnNode = $("<div />").addClass("headerWarning");
        warnNode.append($("<div />").text(updateText));
        $("body").append(warnNode);
    }

    //functions
    $.fn.extend({
        tooltip: function() {
            return this.each(function() {
                $(this).hover(
                        function() {
                            //insert text and object
                            if ($("#tooltip").length < 1)
                                $("body").append($("<div id='tooltip'><div></div><span></span></div>"));
                            $("#tooltip div").html($(this).data("text"));

                            //positioning
                            var pos = $(this).offset();
                            var posLeft = pos.left + 32;
                            var posTop = pos.top;
                            $("#tooltip").css("left", posLeft);
                            $("#tooltip").css("top", posTop);

                            $("#tooltip").show();
                        },
                        function() {
                            $("#tooltip").fadeOut();
                        }
                );

            });
        }
    });


    //ready-state


    //header-related
    $("#slider1").responsiveSlides({
        auto: true,
        pager: true,
        speed: 1000

    });

    $("#slider2").responsiveSlides({
        auto: false,
        pager: true,
        speed: 1000,
        nav: true,
        prevText: '<img alt="" src="/Static/Images/Design/prev1.png">',
        nextText: '<img alt="" src="/Static/Images/Design/next1.png">'

    });
    //form-related
    $("form").not('.quick').submit(function() {

        settingsValidate = {
            ignore: "input[type=date]",
            errorPlacement: function(error, element) {
                error.insertAfter(element);
                error.css('display', 'none');
            },
            success: function(label) {
                label.addClass('valid');
            },
            rules: {
                Email: {
                    email: true
                },
                Phone: {
                    isPhoneNumber: true
                },
                Address: {
                    letterMandatory: true,
                    numberMandatory: true
                },
                PostalCode: {
                    zipcodeNL: true
                },
                Delivery_PostalCode: {
                    zipcodeNL: true
                },
                Zip: {
                    zipcodeNL: true
                }
            }
        };

        $(this).validate(settingsValidate);
        if ($(this).validate().form())
            return true;
        return false;
    });

    $(".tooltip").tooltip();

    $('.mobilemenu a.icon').click(function() {
        $(this).next().slideToggle('fast');
        return false;
    }).next().hide();

    $('.mobilemenu li.drop > a').click(function() {
        $(this).next().slideToggle('fast');
        return false;
    }).next().hide();
});