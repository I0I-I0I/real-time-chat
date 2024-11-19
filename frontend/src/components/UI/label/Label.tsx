import styles from "./Label.module.css"
import cls from "@/utils/cls"

type VariantType =
	| "default"
	| "file"
	| "search"

interface LabelProps extends React.LabelHTMLAttributes<HTMLLabelElement> {
	children: React.ReactNode
	className?: string
	variant?: VariantType
}

export const Label = ({
	children,
	className = "",
	variant = "default",
	...props
}: LabelProps) => {
	return (
		<label className={cls(styles.label, styles[variant], className)} {...props}>
			{children}
		</label>
	)
}
