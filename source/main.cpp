// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

//#define SQL_NOUNICODEMAP

#include "pch.h"
#include "PersonAddressRequest.h"

#define SQL_THROW_IF_FAIL(ret)  \
    if(!SQL_SUCCEEDED(ret))     \
    {                           \
        throw std::exception(); \
    }

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static Microsoft::WRL::ComPtr<ID3D11Device> g_pd3dDevice;
static Microsoft::WRL::ComPtr<ID3D11DeviceContext> g_pd3dDeviceContext;
static Microsoft::WRL::ComPtr<IDXGISwapChain> g_pSwapChain;
static Microsoft::WRL::ComPtr<ID3D11RenderTargetView> g_mainRenderTargetView;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int main(int argc, char** args)
{
    // test code for DBRequest
    DB::PersonAddressRequest request;
    request.Search();

    // SQL CODES BEGIN
    SQLHENV env;
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    SQLRETURN ret;
    //SQLCHAR driver[256];
    //SQLCHAR attr[256];
    //SQLSMALLINT driverLength;
    //SQLSMALLINT attrLength;
    //
    // get information about all drivers
    //while (SQL_SUCCEEDED(ret = SQLDrivers(env, SQL_FETCH_NEXT, driver, 256, &driverLength, attr, 256, &attrLength)))
    //{
    //    std::cout << driver << " - " << attr << std::endl;
    //
    //    if (ret == SQL_SUCCESS_WITH_INFO)
    //    {
    //        std::cout << "data truncation" << std::endl;
    //    }
    //}

    //std::cout << "\nData sources information:" << std::endl;
    //
    //SQLCHAR dsn[256];
    //SQLCHAR desc[256];
    //SQLSMALLINT dsnLength;
    //SQLSMALLINT descLength;
    //
    // get information about data sources
    //while (SQL_SUCCEEDED(ret = SQLDataSources(env, SQL_FETCH_NEXT, dsn, 256, &dsnLength, desc, 256, &descLength)))
    //{
    //    std::cout << dsn << " - " << desc << std::endl;
    //
    //    if (ret == SQL_SUCCESS_WITH_INFO)
    //    {
    //        std::cout << "data truncation" << std::endl;
    //    }
    //}

    SQLCHAR dsnString[] = "DSN=AdventureWorks;";
    SQLCHAR outConnectionString[1024];
    SQLSMALLINT outConnectionStringLength;

    SQLHDBC dbc;
    SQL_THROW_IF_FAIL(SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc));

    if (SQL_SUCCEEDED(SQLDriverConnect(
                    dbc,
                    NULL,
                    dsnString,
                    sizeof(dsnString),
                    outConnectionString,
                    sizeof(outConnectionString),
                    &outConnectionStringLength,
                    SQL_DRIVER_NOPROMPT)))
    {
         std::cout << "\nSuccessfully connected to " << outConnectionString << std::endl;
    }
    else
    {
        std::cout << "Failed to connect" << std::endl;
        return 0;
    }

    // testing an sql statement
    SQLHSTMT stmt;
    SQL_THROW_IF_FAIL(SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt));

    SQL_THROW_IF_FAIL(SQLExecDirect(stmt, (SQLCHAR*)"select top 10 * from Person.Person", SQL_NTS));

    // get amount of columns in the result
    SQLSMALLINT columnCount;
    SQL_THROW_IF_FAIL(SQLNumResultCols(stmt, &columnCount));

    

    int row = 1;
    // fetch each record in the result
    while (SQL_SUCCEEDED(SQLFetch(stmt)))
    {
        std::cout << "Row " << row++ << ":" << std::endl;

        char buf[512];
        SQLLEN indicator;
        // loop through the columns
        for (SQLSMALLINT i = 1; i <= columnCount; ++i)
        {
            SQL_THROW_IF_FAIL(SQLGetData(stmt, i, SQL_C_CHAR, buf, sizeof(buf), &indicator));

            if (indicator == SQL_NULL_DATA)
                strncpy_s(buf, "NULL", 5);

            std::cout << "  Column " << i << ": " << buf << std::endl;
        }
    }

    // close the connection
    if (!SQL_SUCCEEDED(SQLDisconnect(dbc)))
    {
        std::cout << "Failed to disconnect" << std::endl;
    }

    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);
    // SQL CODE END


    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"ImGui Example";
    wc.hIconSm = NULL;

    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(
                    wc.lpszClassName,
                    L"Dear ImGui DirectX11 Example",
                    WS_OVERLAPPEDWINDOW,
                    100, 100,
                    1280, 800,
                    NULL,
                    NULL,
                    wc.hInstance,
                    NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice.Get(), g_pd3dDeviceContext.Get());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use
    // ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application
    // (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double
    // backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    //                              NULL, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code
        // to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");              // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");           // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            // Buttons return true when clicked (most widgets return true when edited/activated)
            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when
            // clicked)
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] =
        {
            clear_color.x * clear_color.w,
            clear_color.y * clear_color.w,
            clear_color.z * clear_color.w,
            clear_color.w
        };

        g_pd3dDeviceContext->OMSetRenderTargets(1, g_mainRenderTargetView.GetAddressOf(), NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView.Get(), clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
                    NULL,
                    D3D_DRIVER_TYPE_HARDWARE,
                    NULL,
                    createDeviceFlags,
                    featureLevelArray,
                    2,
                    D3D11_SDK_VERSION,
                    &sd,
                    g_pSwapChain.GetAddressOf(),
                    g_pd3dDevice.GetAddressOf(),
                    &featureLevel,
                    g_pd3dDeviceContext.GetAddressOf());
    
    if (hr != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, g_mainRenderTargetView.GetAddressOf());
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    g_mainRenderTargetView.Reset();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite
// your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or
// clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
