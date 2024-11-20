import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { createBrowserRouter, RouterProvider } from "react-router-dom"

import HomePage from "@/pages/home/Home"
import LoginPage from "@/pages/login/Login"
import './styles/index.css'

const router = createBrowserRouter([
	{ path: "/", element: <LoginPage />, errorElement: <div>404 Not Found</div> },
	{
		path: "/chat",
		element: <HomePage />,
		children: [
			{
				path: "/chat/:id",
				element: <div></div>
			}
		]
	},
])

createRoot(document.getElementById('root')!).render(
  <StrictMode>
   	<RouterProvider router={router} />
  </StrictMode>,
)
