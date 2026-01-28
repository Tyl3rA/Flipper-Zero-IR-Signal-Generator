#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_infrared.h>
#include <gui/gui.h>
#include <input/input.h>
#include <gui/elements.h>

/* 
 * IR Jammer for Flipper Zero
 * 
 * DISCLAIMER: For educational and testing purposes only.
 */

// Global Jammer State
typedef struct {
    bool is_jamming;
    uint32_t frequency;
} JammerState;

JammerState app_state = {
    .is_jamming = false,
    .frequency = 38000, // Default 38kHz
};

// ISR Callback for Continuous Transmission
static FuriHalInfraredTxGetDataState jammer_tx_data_callback(void* context, uint32_t* duration, bool* level) {
    UNUSED(context);
    *duration = 100000; // 100ms burst duration (longer than loop delay)
    *level = true;      // Keep signal HIGH (carrier ON)
    return FuriHalInfraredTxGetDataStateLastDone;
}

// UI Draw Callback
static void app_draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 10, 10, "IR Jammer");

    canvas_set_font(canvas, FontSecondary);
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Freq: %lu Hz", app_state.frequency);
    canvas_draw_str(canvas, 10, 25, buffer);

    if (app_state.is_jamming) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 30, 45, "JAMMING!!!");
        canvas_draw_circle(canvas, 110, 40, 10);
        canvas_draw_disc(canvas, 110, 40, 5);
    } else {
        canvas_draw_str(canvas, 10, 45, "Press OK to Start");
        canvas_draw_str(canvas, 10, 55, "L/R to Change Freq");
    }
}

// Input Callback
static void app_input_callback(InputEvent* input_event, void* ctx) {
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

// Main Entry Point
int32_t ir_jammer_app(void* p) {
    UNUSED(p);
    
    // Initialize Event Queue
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    
    // Initialize ViewPort
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, app_draw_callback, NULL);
    view_port_input_callback_set(view_port, app_input_callback, event_queue);
    
    // Register ViewPort with GUI
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    
    InputEvent event;
    bool running = true;

    // Main Loop
    while(running) {
        // If jamming is active, send a burst of IR
        if (app_state.is_jamming) {
            // Send a continuous signal (or rapid pulses)
            // Note: furi_hal_infrared_async_tx_* APIs might be better for background transfer,
            // but for a simple jammer, blocking short bursts inside the loop or managing PWM directly is key.
            // CAUTION: Continuous TX can heat up the LED. We will pulse it.
            
            // For simple jamming, we just need to flood the receiver. 
            // Most receivers expect 38kHz.
            
            // Sending a RAW signal: On for 50ms, Off for 10ms (just to let loop breathe)
            // But to jam effectively we want to be "ON" as much as possible to blind the AGC.
            
            furi_hal_infrared_async_tx_set_data_isr_callback(jammer_tx_data_callback, NULL);
            furi_hal_infrared_async_tx_start(app_state.frequency, 0.50f); // 50% duty cycle
            furi_delay_ms(50); // Jam for 50ms
            furi_hal_infrared_async_tx_stop();
            // Tiny gap to process input, though strictly for jamming we'd want 0 gap.
            // But we need to be able to stop it :)
            // Checking queue with 0 timeout allows us to loop tight.
            if(furi_message_queue_get(event_queue, &event, 0) == FuriStatusOk) {
                if(event.key == InputKeyBack) {
                    running = false;
                    app_state.is_jamming = false;
                } else if (event.key == InputKeyOk && event.type == InputTypeShort) {
                    app_state.is_jamming = !app_state.is_jamming;
                }
            }
        } else {
            // Waiting for input
            if(furi_message_queue_get(event_queue, &event, 100) == FuriStatusOk) {
                if(event.type == InputTypeShort || event.type == InputTypeRepeat) {
                    switch(event.key) {
                        case InputKeyBack:
                            running = false;
                            break;
                        case InputKeyOk:
                            app_state.is_jamming = !app_state.is_jamming;
                            break;
                        case InputKeyRight:
                            app_state.frequency += 1000;
                            if (app_state.frequency > 60000) app_state.frequency = 60000;
                            break;
                        case InputKeyLeft:
                            app_state.frequency -= 1000;
                            if (app_state.frequency < 30000) app_state.frequency = 30000;
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        view_port_update(view_port);
    }
    
    // Cleanup
    if (app_state.is_jamming) {
        furi_hal_infrared_async_tx_stop();
    }
    
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    furi_record_close(RECORD_GUI);
    
    return 0;
}
