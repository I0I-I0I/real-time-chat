import { Gradient } from "@/components/UI"
import { Outlet } from "react-router"

const AuthLayout = () => {
	return (
		<>
			<Gradient />
            <Outlet />
		</>
	)
}

export default AuthLayout
