<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<div class="form-container">
    <form action="" class="form-container__form">
        <div class="form-coordinate-y NOT-GLOBAL">
            <div class="form-coordinate-y__wrapper">
                <div class="form-coordinate-y__item">
                    <span class="form-coordinate-y__title">Enter coordinate y:</span>
                    <input type="text" name="coordinate-y" placeholder="0"/>
                </div>
            </div>
        </div>
        <div class="form-radius NOT-GLOBAL">
            <div class="form-radius__wrapper">
                <span class="form-radius__title">Choose the radius:</span>
                <div class="form-radius__item">
                    <input type="button" value="1"/>
                </div>
                <div class="form-radius__item">
                    <input type="button" value="2"/>
                </div>
                <div class="form-radius__item">
                    <input type="button" value="3"/>
                </div>
                <div class="form-radius__item">
                    <input type="button" value="4"/>
                </div>
                <div class="form-radius__item">
                    <input type="button" value="5"/>
                </div>
            </div>
        </div>
        <div class="form-coordinate-x NOT-GLOBAL">
            <div class="form-coordinate-x__wrapper">
                <span class="form-coordinate-x__title">Choose coordinate x:</span>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="-2"/>
                    <div class="wrapper-span"> <!--to set position-relative-->
                        <span>-2</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="-1.5"/>
                    <div class="wrapper-span">
                        <span>-1.5</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="-1"/>
                    <div class="wrapper-span">
                        <span>-1</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="-0.5"/>
                    <div class="wrapper-span">
                        <span>-0.5</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="0"/>
                    <div class="wrapper-span">
                        <span>0</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="0.5"/>
                    <div class="wrapper-span">
                        <span>0.5</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="1"/>
                    <div class="wrapper-span">
                        <span>1</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="1.5"/>
                    <div class="wrapper-span">
                        <span>1.5</span>
                    </div>
                </label>
                <label class="form-coordinate-x__item">
                    <input name="coordinate-x" type="radio" value="2"/>
                    <div class="wrapper-span">
                        <span>2</span>
                    </div>
                </label>
            </div>
        </div>
        <div class="client-button">
            <div class="client-button__wrapper">
                <input class="client-button__clear" type="button" value="Clear table"/>
                <input class="client-button__submit" type="button" value="Submit"/>
            </div>
        </div>
    </form>
</div>
